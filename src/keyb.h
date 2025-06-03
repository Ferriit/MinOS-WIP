#ifndef KEYB_LIB_H
#define KEYB_LIB_H

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

int scancode_to_ascii(unsigned char scancode);

void CodeBufferToAscii(const char* in, char* out);

unsigned char GetKeyboardScancode();

char GetKeyboardInput();

#endif
