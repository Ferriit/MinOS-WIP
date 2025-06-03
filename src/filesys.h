#ifndef FILESYS_H
#define FILESYS_H

// Type aliases (if <stdint.h> is unavailable)
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;

#define SECTOR_SIZE 512
#define MAX_FILENAME_LEN 11

typedef struct {
    char filename[8];
    char extension[3];
    uint8_t attributes;
    uint8_t reserved[10];
    uint16_t time;
    uint16_t date;
    uint16_t start_cluster;
    uint32_t size;
} __attribute__((packed)) FAT16Entry;

// Disk I/O – to be implemented elsewhere
void read_sector(uint32_t lba, void* buffer);
void write_sector(uint32_t lba, const void* buffer);

// File access interface
int K_ReadFile(const char* name, char* out_buffer);
int K_WriteFile(const char* name, const char* data, int size);

#endif // FILESYS_H
