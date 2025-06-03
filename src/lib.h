#ifndef KERNEL_LIB_H
#define KERNEL_LIB_H

int CountOccurrences(char* string, char element);

void AppendSubstring(char* dest, const char* src);

char* ClampSizeFromBeginning(char* string, int size);

int StringLength(char* length);

unsigned int strlen_custom(const char* s);

char* strstr_custom(char* haystack, const char* needle);

void memmove_custom(char* dest, const char* src, unsigned int count);

void itoa(int value, char* str);

void copy_string(char* dest, const char* src);

char* FixToLineAmount(char* string, int maxLines);

void* memcpy_custom(void* dest, const void* src, unsigned int n);

void* memset_custom(void* dest, int val, unsigned int n);

int memcmp_custom(const void* s1, const void* s2, unsigned int n);

#endif
