#include <stdint.h> /* intmax_t */

#include "stdc/stdarg.h"
#include "sbi.h"

// needs to be defined elsewhere
void putchar(long ch);

void puts(const char *s) {
  if(!s) return;
  for(; *s; s++) putchar(*s);
}

struct number_fmt {
  uint8_t lengthSpecifier; /* minimum length, pad with spaces or 0s */
  bool printPrefix; /* print '0b', '0o', '0x' */
  bool zeroPad; /* pad with 0s */
  bool printSign; /* print '+' when positive */
  bool hexUppercase; /* print the hex characters in lowercase */
};

bool print_number(size_t number, uint8_t base, bool is_signed, struct number_fmt fmt)
{
  const char *digits = fmt.hexUppercase ? "0123456789ABCDEF" : "0123456789abcdef";
  char buffer[68]; /* ['+'|'-'] '0b' + 64bits + '\0' */
  bool negative = false;
  size_t i = 0;

  if(base > 16) return false;

  if(is_signed) {
    if((intmax_t)number < 0) {
      number = -(intmax_t)number;
      negative = true;
    }
  }

  do {
    buffer[i++] = digits[number % base];
    number /= base;
  } while(number > 0);

  if(fmt.zeroPad)
    while(i < fmt.lengthSpecifier) buffer[i++] = '0'; /* pad with 0s */
  else
    while(i < fmt.lengthSpecifier) buffer[i++] = ' '; /* pad with spaces */

  if(fmt.printPrefix) {
    switch(base) {
      case 2: /* Binary */
        buffer[i++] = 'b';
        buffer[i++] = '0';
        break;
      case 8: /* Octal */
        buffer[i++] = 'o';
        buffer[i++] = '0';
        break;
      case 10: /* Decimal */ break;
      case 16: /* Hexadecimal */
        buffer[i++] = 'x';
        buffer[i++] = '0';
        break;
      default: /* Invalid ? */
        break;
    }
  }

  if(negative) buffer[i++] = '-';
  else if(is_signed && fmt.printSign) buffer[i++] = '+';
  buffer[i--] = '\0';

  /* Reverse digits in buffer */
  for(size_t j = 0; j < i; j++, i--) {
    char temp = buffer[j];
    buffer[j] = buffer[i];
    buffer[i] = temp;
  }

  puts(buffer);
  return true;
}

void vprintf(const char *fmt, va_list vargs)
{
  char fmtBuf[6] = {0};
  int fmtBufIdx;
  for(;*fmt; fmt++) {
    if(*fmt == '%') {
      fmt++;
      fmtBufIdx = 0;

      struct number_fmt numFmt = {
        .lengthSpecifier = 0,
        .printPrefix = false,
        .zeroPad = false,
        .printSign = false,
        .hexUppercase = false,
      };

      // flags
      for(int i = 0; i < 3; i++) {
        if(*fmt == '#') {
          numFmt.printPrefix = true;
          fmtBuf[fmtBufIdx++] = '#';
          fmt++;
        } else if(*fmt == '0') {
          numFmt.zeroPad = true;
          fmtBuf[fmtBufIdx++] = '0';
          fmt++;
        } else if(*fmt == '+') {
          numFmt.printSign = true;
          fmtBuf[fmtBufIdx++] = '+';
          fmt++;
        } else {
          break;
        }
      }

      fmtBuf[fmtBufIdx++] = *fmt;
      switch(*fmt) {
        case '\0': {
          putchar('%');
          goto end;
        } break;
        
        case '%': {
          putchar('%');
        } break;

        case 'c': {
          int c = va_arg(vargs, int);
          putchar(c);
        } break;

        case 'd': {
          int d = va_arg(vargs, int);
          print_number((size_t)d, 10, true, numFmt);
        } break;

        case 'o': {
          int o = va_arg(vargs, int);
          print_number((size_t)o, 8, false, numFmt);
        } break;

        case 's': {
          const char *s = va_arg(vargs, const char*);
          puts(s);
        } break;

        case 'u': {
          unsigned int u = va_arg(vargs, unsigned int);
          print_number((size_t)u, 10, false, numFmt);
        } break;

        case 'x': {
          unsigned int x = va_arg(vargs, unsigned int);
          print_number((size_t)x, 16, false, numFmt);
        } break;

        default: {
          fmtBuf[fmtBufIdx] = '\0';
          puts("Invalid format specifier '%");
          puts(fmtBuf);
          puts("'\r\n");
          goto end;
        } break;
      }
    } else {
      putchar(*fmt);
    }
  }
end:
}

void printf(const char *fmt, ...)
{
  va_list vargs;
  va_start(vargs, fmt);
  vprintf(fmt, vargs);
  va_end(vargs);
}
