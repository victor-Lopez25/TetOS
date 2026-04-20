#include "kernel.h"

#include "stdc/string.c"
#include "printf.c"

extern char __bss[], __bss_end[], __stack_top[];

extern void boot(void);
extern void kernel_entry(void);

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid) {
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  __asm__ __volatile__("ecall"
                       : "=r"(a0), "=r"(a1)
                       : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                         "r"(a6), "r"(a7)
                       : "memory");
  return (struct sbiret){.error = a0, .value = a1};
}

void putchar(long ch) {
  sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

void PrintTeto(void) {
#if 0
  printf("               r               \n");
  printf("________  ------------  ________\n");
  printf("\      / /            \ \      /\n");
  printf(" \    / /  __/  _/\__  \ \    /\n");
  printf("  \  / /__/ /__/     \__\ \  /\n");
  printf("   \/                      \/\n");
#else
  printf("               r\n");
  printf("________  ------------  ________\n");
  printf("\\      / /            \\ \\      /\n");
  printf(" \\    / /  __/  _/\\__  \\ \\    /\n");
  printf("  \\  / /__/ /__/     \\__\\ \\  /\n");
  printf("   \\/                      \\/\n");
#endif
}

void handle_trap(struct trap_frame *f) {
  (void)f;
  uint32_t scause = READ_CSR(scause);
  uint32_t stval = READ_CSR(stval);
  uint32_t user_pc = READ_CSR(sepc);

  PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}

void kernel_main(void) {
  memset(__bss, 0, (size_t) __bss_end - (size_t) __bss);
  // will this work in 64 bit?
  WRITE_CSR(stvec, kernel_entry, size_t);

  printf("\n\nHello %s\n", "World!");
  printf("1 + 2 = %d, %x\n", 1 + 2, 0x1234abcd);
  printf("%+d, %#x\n", 2465, 0xabcd);
  printf("%+#0e\n", 0xabcd);
  PrintTeto();

  //PANIC("TETO destroy!!");
  __asm__ __volatile__("unimp");

  for (;;) {
    __asm__ __volatile__("wfi");
  }
}
