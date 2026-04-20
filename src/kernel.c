#include "kernel.h"

#include "stdc/string.c"
#include "printf.c"

#include "sbi/sbi.c"

extern char __bss[], __bss_end[], __stack_top[];

extern void boot(void);
extern void kernel_entry(void);

void putchar(long ch) {
  sbi_console_putchar(ch);
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

void test_sbi(void)
{
  struct sbiret sbiSpecVersion = sbi_get_spec_version();
  printf("Sbi Spec version: %d.%d\n", 
         (sbiSpecVersion.uvalue >> 24), sbiSpecVersion.uvalue & 0xFFFFFF);

  struct sbiret sbiImplId = sbi_get_impl_id();
  printf("Sbi Impl id: %d\n", sbiImplId.uvalue);

  bool DBCN_ok = sbi_probe_extension(SBI_DBCN_EID).uvalue == 1;
  printf("Debug Console Extension EID: %s\n", DBCN_ok ? "available" : "unavailable");
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

  test_sbi();

  //PANIC("TETO destroy!!");
  // unimp = illegal instruction
  //__asm__ __volatile__("unimp");

  printf("Done everything, shutting down...\n");
  sbi_shutdown();

  for(;;) {
    // wfi = wait for interrupt
    __asm__ __volatile__("wfi");
  }
}
