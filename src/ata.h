// I/O port addresses for primary ATA bus
#define ATA_REG_DATA        0x1F0
#define ATA_REG_ERROR       0x1F1
#define ATA_REG_FEATURES    0x1F1
#define ATA_REG_SECCOUNT0   0x1F2
#define ATA_REG_LBA0        0x1F3
#define ATA_REG_LBA1        0x1F4
#define ATA_REG_LBA2        0x1F5
#define ATA_REG_HDDEVSEL    0x1F6
#define ATA_REG_COMMAND     0x1F7
#define ATA_REG_STATUS      0x1F7

// Status flags
#define ATA_SR_BSY      0x80
#define ATA_SR_DRDY     0x40
#define ATA_SR_DF       0x20
#define ATA_SR_DSC      0x10
#define ATA_SR_DRQ      0x08
#define ATA_SR_CORR     0x04
#define ATA_SR_IDX      0x02
#define ATA_SR_ERR      0x01

// ATA commands
#define ATA_CMD_READ_PIO     0x20
#define ATA_CMD_WRITE_PIO    0x30
