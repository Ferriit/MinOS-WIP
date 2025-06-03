#include "lib.h"
#define WIDTH 80
#define HEIGHT 25

int CountOccurrences(char* string, char element) {
    int count = 0;
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == element) {
            count++;
        }
    }
    return count;
}


// Simple strlen implementation
unsigned int strlen_custom(const char* s) {
    unsigned int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}


// Simple strstr implementation to find substring
char* strstr_custom(char* haystack, const char* needle) {
    if (*needle == '\0') return haystack;  // empty substring matches start

    for (; *haystack != '\0'; haystack++) {
        char *h = haystack;
        const char *n = needle;

        while (*h != '\0' && *n != '\0' && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return haystack;  // found substring
        }
    }
    return 0;  // not found
}


// Simple memmove implementation
void memmove_custom(char* dest, const char* src, unsigned int count) {
    if (dest < src) {
        for (unsigned int i = 0; i < count; i++) {
            dest[i] = src[i];
        }
    } else {
        for (int i = count - 1; i >= 0; i--) {
            dest[i] = src[i];
        }
    }
}


char* ClampSizeFromBeginning(char* string, int size) {
    int stringsize = StringLength(string);
    if (stringsize > size) {
        int diff = stringsize - size;

        // Shift characters left by diff
        for (int i = 0; i <= size; i++) {
            string[i] = string[i + diff];
        }
    }
    return string;
}


void AppendSubstring(char* dest, const char* src) {
    int dest_len = strlen_custom(dest);
    int src_len = strlen_custom(src);

    // Ensure there's space (minus 1 for null terminator)
    int space = WIDTH * HEIGHT - dest_len - 1;
    if (space <= 0) return;

    int copy_len = (src_len < space) ? src_len : space;

    for (int i = 0; i < copy_len; i++) {
        dest[dest_len + i] = src[i];
    }

    dest[dest_len + copy_len] = '\0';
}


void* memcpy_custom(void* dest, const void* src, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    for (unsigned int i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}


void* memset_custom(void* dest, int val, unsigned int n) {
    unsigned char* d = (unsigned char*)dest;

    for (unsigned int i = 0; i < n; i++) {
        d[i] = (unsigned char)val;
    }

    return dest;
}


int memcmp_custom(const void* s1, const void* s2, unsigned int n) {
    const unsigned char* a = (const unsigned char*)s1;
    const unsigned char* b = (const unsigned char*)s2;

    for (unsigned int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
    }

    return 0;
}



char* FixToLineAmount(char* string, int maxLines) {
    int count = 0;

    // Count lines from the end backwards
    for (int i = strlen_custom(string) - 1; i >= 0; i--) {
        if (string[i] == '\n') {
            count++;
            if (count == maxLines) {
                // Shift remaining string to the front
                int newStart = i + 1;
                int j = 0;
                while (string[newStart]) {
                    string[j++] = string[newStart++];
                }
                string[j] = '\0';  // null-terminate
                break;
            }
        }
    }

    return string;
}


int StringLength(char* length) {
    int count = 0;
    for (int i = 0; length[i] != '\0'; i++) {
        count += 1;
    }

    return count;
}


void copy_string(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}


void itoa(int value, char* str) {
    int i = 0;
    int is_negative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // Process digits in reverse order
    while (value != 0) {
        int digit = value % 10;
        str[i++] = digit + '0';
        value /= 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Reverse the string
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}
