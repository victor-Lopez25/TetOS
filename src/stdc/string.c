#include "archdetect.h"
#include "string.h"

char *strcpy(char *dst, const char *src)
{
  if(!dst || !src) return 0;
  char *d = dst;
  const char *s = src;
  while(*s) *d++ = *s++;
  *d = '\0';
  return dst;
}

size_t strlen(const char *s)
{
  if(!s) return 0;
  const char *sIni = s;
  while(*s++);
  return s - sIni;
}

int strcmp(const char *s1, const char *s2)
{
  // cast to unsigned char* for POSIX
  while(*s1 && *s2) {
    if(*s1 != *s2) break;
    s1++; s2++;
  }
  return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
  // cast to unsigned char* for POSIX
  for(size_t i = 0; i < n && *s1 && *s2; i++) {
    if(s1[i] != s2[i]) break;
  }
  return *(unsigned char*)s1 - *(unsigned char*)s2;
}

#define MEMCPY_ALIGN (sizeof(size_t)-1)
void *memcpy(void *dst, const void *src, size_t len)
{
  uint8_t *d = (uint8_t*)dst;
  const uint8_t *s = (const uint8_t*)src;
  if(((uintptr_t)d & MEMCPY_ALIGN) != ((uintptr_t)s & MEMCPY_ALIGN))
      goto misaligned;

  for(; ((uintptr_t)d & MEMCPY_ALIGN) && len != 0; len--) *d++ = *s++;
  if(len != 0) {
      size_t *wideDst = (size_t*)d;
      const size_t *wideSrc = (const size_t*)s;

      for(; len >= sizeof(size_t); len -= sizeof(size_t)) *wideDst++ = *wideSrc++;
      d = (uint8_t*)wideDst;
      s = (const uint8_t*)wideSrc;
misaligned:
      for(; len != 0; len--) *d++ = *s++;
  }

  return dst;
}
#undef MEMCPY_ALIGN

void memmove(void *dst, const void *src, size_t len)
{
  uint8_t *d = (uint8_t*)dst;
  const uint8_t *s = (const uint8_t*)src;

  if((uintptr_t)dst == (uintptr_t)src) return;
  if((uintptr_t)s-(uintptr_t)d-len <= -2*len) {
    memcpy(d, s, len);
    return;
  }

  if(d < s) {
#if defined(__GNUC__) || defined(__GNUG__)
    if((uintptr_t)s % sizeof(size_t) == (uintptr_t)d % sizeof(size_t)) {
      while((uintptr_t)d % sizeof(size_t)) {
        if(!len--) return;
        *d++ = *s++;
      }
      __attribute__((__may_alias__)) size_t *wideDst = (size_t*)d;
      __attribute__((__may_alias__)) size_t *wideSrc = (size_t*)s;
      for(; len >= sizeof(size_t); len -= sizeof(size_t)) *d++ = *s++;
      d = (uint8_t*)wideDst;
      s = (const uint8_t*)wideSrc;
    }
#endif
    for(; len; len--) *d++ = *s++;
  } else {
#if defined(__GNUC__) || defined(__GNUG__)
    if((uintptr_t)s % sizeof(size_t) == (uintptr_t)d % sizeof(size_t)) {
      while((uintptr_t)(d + len) % sizeof(size_t)) {
        if(!len--) return;
        d[len] = s[len];
      }
      while(len >= sizeof(size_t)) {
        len -= sizeof(size_t);
        *(__attribute__((__may_alias__)) size_t*)(d + len) = *(__attribute__((__may_alias__)) size_t*)(s + len);
      }
    }
#endif
    while(len) len--, d[len] = s[len];
  }
}

void *memset(void *buf, int ch, size_t len)
{
  size_t i;

  if ((uintptr_t)buf % sizeof(size_t) == 0 &&
    len % sizeof(size_t) == 0) {
    size_t *d = (size_t*)buf;
    ch &= 0xFF; // make sure to take only the first byte
#if ARCH_64BIT
    size_t copied = ch | (ch << 8) | (ch << 16) | (ch << 24) |
                    ((size_t)ch << 32) | ((size_t)ch << 40) | ((size_t)ch << 48) | ((size_t)ch << 56);
#elif ARCH_32BIT
    size_t copied = ch | (ch << 8) | (ch << 16) | (ch << 24);
#endif

    for(i = 0; i < len/sizeof(size_t); i++) {
      d[i] = copied;
    }
  }
  else {
    char *d = (char*)buf;

    for(i = 0; i < len; i++) {
      d[i] = ch;
    }
  }
  return buf;
}

int memcmp(const void *str1, const void *str2, size_t count)
{
  const unsigned char *s1 = (const unsigned char*)str1;
  const unsigned char *s2 = (const unsigned char*)str2;

  for(;count-- > 0;) {
    if(*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
  }
  return 0;
}