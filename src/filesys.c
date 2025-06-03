#include "filesys.h"
#include "lib.h"
#include "ata.h"

#define SECTOR_SIZE 512
#define FAT_ENTRY_SIZE 2
#define MAX_FILENAME_LEN 11  // 8.3 filename format

#define ROOT_DIR_SECTOR 19      // Depends on your FS layout
#define ROOT_DIR_ENTRIES 512
#define ROOT_DIR_SIZE (ROOT_DIR_ENTRIES * 32 / SECTOR_SIZE)
#define FAT1_SECTOR 1
#define FAT_SECTORS 9
#define CLUSTER_START_SECTOR (ROOT_DIR_SECTOR + ROOT_DIR_SIZE)

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;


uint8_t sector_buffer[512];


int K_ReadFile(const char* name, char* out_buffer) {
    FAT16Entry entry;
    for (int i = 0; i < ROOT_DIR_SIZE; i++) {
        read_sector(ROOT_DIR_SECTOR + i, sector_buffer);

        for (int j = 0; j < SECTOR_SIZE; j += 32) {
            FAT16Entry* e = (FAT16Entry*)&sector_buffer[j];

            if (e->filename[0] == 0x00) return -1; // No more entries
            if ((e->attributes & 0x0F) == 0x0F) continue; // Skip long names

            char filename[12];
            for (int k = 0; k < 8; k++) filename[k] = e->filename[k];
            for (int k = 0; k < 3; k++) filename[8 + k] = e->extension[k];
            filename[11] = '\0';

            if (memcmp_custom(filename, name, 11) == 0) {
                memcpy_custom(&entry, e, sizeof(FAT16Entry));
                goto found;
            }
        }
    }

    return -1; // Not found

found:
    uint16_t cluster = entry.start_cluster;
    uint32_t bytes_read = 0;
    while (cluster < 0xFFF8) {
        uint32_t lba = CLUSTER_START_SECTOR + (cluster - 2);
        read_sector(lba, &out_buffer[bytes_read]);
        bytes_read += SECTOR_SIZE;

        read_sector(FAT1_SECTOR + (cluster * 2 / SECTOR_SIZE), sector_buffer);
        cluster = *(uint16_t*)&sector_buffer[(cluster * 2) % SECTOR_SIZE];
    }

    return bytes_read;
}


int K_WriteFile(const char* name, const char* data, int size) {
    uint16_t cluster = 2; // First usable cluster
    int remaining = size;
    int written = 0;

    while (remaining > 0) {
        uint32_t lba = CLUSTER_START_SECTOR + (cluster - 2);
        write_sector(lba, &data[written]);
        written += SECTOR_SIZE;
        remaining -= SECTOR_SIZE;

        // Update FAT table
        uint16_t next = (remaining > 0) ? (cluster + 1) : 0xFFFF;
        read_sector(FAT1_SECTOR + (cluster * 2 / SECTOR_SIZE), sector_buffer);
        *(uint16_t*)&sector_buffer[(cluster * 2) % SECTOR_SIZE] = next;
        write_sector(FAT1_SECTOR + (cluster * 2 / SECTOR_SIZE), sector_buffer);

        cluster = next;
    }

    // Write root directory entry
    FAT16Entry entry = {0};
    memset_custom(entry.filename, ' ', 11);
    memcpy_custom(entry.filename, name, strlen_custom(name));
    entry.start_cluster = 2;
    entry.size = size;

    for (int i = 0; i < ROOT_DIR_SIZE; i++) {
        read_sector(ROOT_DIR_SECTOR + i, sector_buffer);
        for (int j = 0; j < SECTOR_SIZE; j += 32) {
            FAT16Entry* e = (FAT16Entry*)&sector_buffer[j];
            if (e->filename[0] == 0x00 || e->filename[0] == 0xE5) {
                memcpy_custom(e, &entry, sizeof(FAT16Entry));
                write_sector(ROOT_DIR_SECTOR + i, sector_buffer);
                return 0;
            }
        }
    }

    return -1; // No free entry
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

void io_wait() {
    outb(0x80, 0);
}


void ata_wait_bsy() {
    while (inb(ATA_REG_STATUS) & ATA_SR_BSY);
}

void ata_wait_drq() {
    while (!(inb(ATA_REG_STATUS) & ATA_SR_DRQ));
}

void read_sector(uint32_t lba, void* buffer) {
    uint8_t* buf = (uint8_t*)buffer;

    ata_wait_bsy();

    outb(ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));  // Master + LBA
    outb(ATA_REG_SECCOUNT0, 1);  // One sector
    outb(ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ata_wait_bsy();
    ata_wait_drq();

    // Read 512 bytes (256 words)
    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(ATA_REG_DATA);
        buf[i * 2] = data & 0xFF;
        buf[i * 2 + 1] = (data >> 8);
    }

    io_wait();
}


void write_sector(uint32_t lba, const void* buffer) {
    const uint8_t* buf = (const uint8_t*)buffer;

    ata_wait_bsy();

    outb(ATA_REG_HDDEVSEL, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_REG_SECCOUNT0, 1);
    outb(ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        uint16_t word = buf[i * 2] | (buf[i * 2 + 1] << 8);
        outw(ATA_REG_DATA, word);
    }

    // Flush the cache
    outb(ATA_REG_COMMAND, 0xE7);  // CACHE FLUSH
    ata_wait_bsy();
    io_wait();
}
