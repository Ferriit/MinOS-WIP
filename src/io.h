#ifndef IO_KERNEL_H
#define IO_KERNEL_H

static inline uint8_t inb(uint16_t port);

static inline void outb(uint16_t port, uint8_t val);

static inline uint16_t inw(uint16_t port);

static inline void outw(uint16_t port, uint16_t val);

#endif
