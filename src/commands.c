#include "commands.h"
#include "filesys.h"
#include "lib.h"

#define WIDTH 80
#define HEIGHT 25

/*
TAGS:
    k_      Kernel-related commands
    r_      Restricted commands
    u_      User-space available commands
*/


char textbuffer[HEIGHT * WIDTH] = {0}; // all zeroes, first char is '\0'
char usercommand[256];
volatile char* video = (volatile char*)0xB8000;

//int f_write()

void u_clear_screen() {
    volatile char* video = (volatile char*)0xB8000;
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        video[i * 2] = ' ';        // space character
        video[i * 2 + 1] = 0x07;   // normal attribute (light grey on black)
    }
}


void r_printraw(char* string, int pos) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '\n') {
            // Move to start of next line
            pos += WIDTH - (pos % WIDTH);
        } else if (string[i] == '\t') {
            for (int i = 0; i < 4; i++) {
                video[pos * 2] = ' ';       // character
                video[pos * 2 + 1] = 0x07;         // color attribute
                pos++;
            }
        }
        else {
            video[pos * 2] = string[i];       // character
            video[pos * 2 + 1] = 0x07;         // color attribute
            pos++;
        }
    }
}


void u_print(char* string) {
    AppendSubstring(textbuffer, string);
    ClampSizeFromBeginning(textbuffer, WIDTH * HEIGHT);
    FixToLineAmount(textbuffer, HEIGHT);

    u_clear_screen();
    r_printraw(textbuffer, 0);
}


void u_readinput(char* buf) {
    copy_string(buf, usercommand);
}


void u_clearinputbuffer() {
    for (int j = 0; usercommand[j] != '\0'; j++) {
        usercommand[j] = '\0';
    }
}
