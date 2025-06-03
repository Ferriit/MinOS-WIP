#ifndef KERNEL_COMMANDS_H
#define KERNEL_COMMANDS_H

#define WIDTH 80
#define HEIGHT 25

extern char usercommand[256];

extern volatile char* video;

extern char textbuffer[HEIGHT * WIDTH];

void r_printraw(char* string, int pos);

void u_print(char* string);

void u_readinput(char* buf);

void u_clearinputbuffer();

void u_clear_screen();

#endif
