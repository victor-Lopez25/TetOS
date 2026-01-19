#ifndef _STRING_H
#define _STRING_H

#include "stddef.h"

char *strcpy(char *dst, const char *src);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

void *memset(void *buf, int ch, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
int memcmp(const void *str1, const void *str2, size_t count);

#endif // _STRING_H