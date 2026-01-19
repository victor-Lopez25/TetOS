ENTRY(boot)

SECTIONS {
  . = 0x80200000;

  .text :{
    KEEP(*(.text.boot));
    *(.text .text.*);
  }

  .rodata : ALIGN(4) {
    *(.rodata .rodata.*);
  }

  .data : ALIGN(4) {
    *(.data .data.*);
  }

  .bss : ALIGN(4) {
    PROVIDE(__bss = .);
    *(.bss .bss.* .sbss .sbss.*);
    PROVIDE(__bss_end = .);
  }

  . = ALIGN(4);
  . += 128 * 1024; /* 128KB */
  PROVIDE(__stack_top = .);
}