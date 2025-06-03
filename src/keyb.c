#include "keyb.h"

int HeldKeys[16];

int scancode_to_ascii(unsigned char scancode) {
    switch (scancode) {
        // Letters (A-Z)
        case 0x1E: return 'A';
        case 0x30: return 'B';
        case 0x2E: return 'C';
        case 0x20: return 'D';
        case 0x12: return 'E';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x17: return 'I';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x32: return 'M';
        case 0x31: return 'N';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x10: return 'Q';
        case 0x13: return 'R';
        case 0x1F: return 'S';
        case 0x14: return 'T';
        case 0x16: return 'U';
        case 0x2F: return 'V';
        case 0x11: return 'W';
        case 0x2D: return 'X';
        case 0x15: return 'Y';
        case 0x2C: return 'Z';

        // Numbers (top row, not numpad)
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';

        // Space
        case 0x39: return ' ';

        // Enter
        case 0x1C: return '\n';

        case 0x0F: return '\t';     // Tab

        // Backspace
        case 0x0E: return -1;

        // Nordic layout punctuation:
        case 0x34: return '.';   // PERIOD key (VK_OEM_PERIOD)
        case 0x33: return ',';   // COMMA key (VK_OEM_COMMA)
        case 0x0C: return '+';   // PLUS key (VK_OEM_PLUS)

        case 0x35: return '-';

        case 0x1D: return -3;       // Left Control

        case 0x5B: return -2;       // Left Sys button


        case 0x48: return -4;       // Arrow-up

        case 0x4B: return -5;       // Arrow-left

        case 0x50: return -6;       // Arrow-down

        case 0x4D: return -7;       // Arrow-right


        default:
            return 0; // No mapping / unhandled key
    }
}


unsigned char GetKeyboardScancode() {
    // Wait until data is available
    while ((inb(0x64) & 1) == 0) {
        // wait (busy loop)
    }
    // Read and return the scancode
    return inb(0x60);
}


void CodeBufferToAscii(const char* in, char* out) {
    for (int i = 0; i < 80 * 25 + 1; i++) {
        if (in[i] == -2)
            out[i] = '~';       // Tilde for SYS
        else if (in[i] == -3)
            out[i] = '^';       // Caret for CTRL
        else if (in[i] == -4)
            out[i] = '↑';
        else if (in[i] == -5)
            out[i] = '←';
        else if (in[i] == -6)
            out[i] = '↓';
        else if (in[i] == -7)
            out[i] = '→';
        else
            out[i] = in[i];
    }
}



char GetKeyboardInput() {
    unsigned char scancode = GetKeyboardScancode();

    // Ignore break codes (if high bit set, it's a break code)
    if (scancode & 0x80) {
        return 0;
    }

    return scancode_to_ascii(scancode);
}
