#define VL_BUILD_IMPLEMENTATION
#include "vl_build.h"

#define RISCV_PREFIX "riscv64-unknown-elf-"
#define RISCV_gcc RISCV_PREFIX "gcc"
#define RISCV_as RISCV_PREFIX "as"
#define RISCV_ld RISCV_PREFIX "ld"

#define FREESTANDING "-nostdlib", "-ffreestanding", "-nostartfiles"

#define OUTPUT_DIR "bin"

int main(int argc, char **argv)
{
  VL_GO_REBUILD_URSELF(argc, argv);

  MkdirIfNotExist(OUTPUT_DIR);

  vl_cmd cmd = {0};
  cmd_Append(&cmd, RISCV_gcc, "-c", "src/kernel.c", "-o", "bin/kernel.o",
             "-mcmodel=medany", FREESTANDING,
             "-g3");
  VL_ccWarnings(&cmd, CCompiler_GCC);
  if(!CmdRun(&cmd)) {
    fprintf(stderr, "Failed to compile kernel\n");
    return 1;
  }

  VL_Pushd(OUTPUT_DIR);
  cmd_Append(&cmd, RISCV_gcc, "-T", "../src/kernel.ld");
  cmd_Append(&cmd, "kernel.o", "-o", "kernel.elf", FREESTANDING);
  if(!CmdRun(&cmd)) {
    fprintf(stderr, "Link failed\n");
    return 1;
  }    

  cmd_Append(&cmd, "qemu-system-riscv64",
             "-machine", "virt",
             "-bios", "default",
             "-nographic",
             "-kernel", "kernel.elf",
             "-serial", "mon:stdio",
             "--no-reboot"
  );
  if(!CmdRun(&cmd)) {
    fprintf(stderr, "qemu execution failed\n");
    return 1;
  }

  VL_Popd();

  return 0;
}