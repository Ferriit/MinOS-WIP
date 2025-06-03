// IMPORTING LIBRARIES

// DEFINING STUFF THAT IS DEFINED IN STANDARD C LIBRARIES
typedef unsigned char BOOL;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

// IMPORT ASSEMBLY LIBS
extern void srand(unsigned int seed);
extern unsigned int rand(void);
extern unsigned int time(void);
extern void wait_cycles(unsigned int cycles);
unsigned long long measure_frequency(void);


// IMPORT C LIBS
#include "lib.h"
#include "keyb.h"
#include "commands.h"
#include "filesys.h"
#include "commands.h"

#define true 1
#define false 0

char* message;


void start_message() {
    unsigned int r = rand() % 6;

    static char possiblemessages[6][256] = {"\"Judgement!\"\n\n",
                                "\"Thy end is now!\"\n\n",
                                "\"Prepare thyself!\"\n\n",
                                "\"Forgive me my children, for I have failed to bring you salvation from this cold, dark world...\"\n\n",
                                "\"Ah... free at last. O Gabriel... now dawns thy reckoning, and thy gore shall glisten before the temples of man!\"\n\n",
                                "\"Creature of steel, my gratitude upon thee for my freedom. But the crimes thy kind have committed against humanity are not forgotten! And thy punishment... is death!\"\n\n"};


    message = possiblemessages[r];
    //u_print(message);
}


void printcommand(char* usercommand, char* textbuf, int offset) {
    u_clear_screen();
    r_printraw(message, 0);
    CodeBufferToAscii(usercommand, textbuf);
    r_printraw(textbuf, offset);
}


void kernel_main() {
    unsigned int seed = time();
    srand(seed);

    textbuffer[0] = '\0';  // Start empty
    u_clear_screen();

    start_message();

    char textbuf[20 * 85];

    int cursorptr = 0;

    int offset = (CountOccurrences(message, '\n') + 1 + (int)StringLength(message) / WIDTH) * WIDTH;

    r_printraw(message, 0);

    while (true) {
        char typed = GetKeyboardInput();
        if (typed != 0 && typed != -1 && typed != '\n') { // normal char
            if (cursorptr < sizeof(usercommand) - 1) {
                usercommand[cursorptr] = typed;
                cursorptr++;
                usercommand[cursorptr] = '\0';  // Null-terminate after the new char
            }
            printcommand(usercommand, textbuf, offset);
        }
        else if (typed == -1) { // backspace
            if (cursorptr > 0) {
                cursorptr--;
                usercommand[cursorptr] = '\0';  // Remove last char
                printcommand(usercommand, textbuf, offset);
            }
        }
        else if (typed == '\n') {
            cursorptr = 0;

            for (int j = 0; usercommand[j] != '\0'; j++) {
                usercommand[j] = '\0';
            }

            printcommand(usercommand, textbuf, offset);
        }
    }
}
