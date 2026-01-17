#ifndef SBI_H
#define SBI_H

// https://github.com/riscv-non-isa/riscv-sbi-doc
#define SBI_SUCCESS 0                /* Completed successfully */
#define SBI_ERR_FAILED -1            /* Failed */
#define SBI_ERR_NOT_SUPPORTED -2     /* Not supported */
#define SBI_ERR_INVALID_PARAM -3     /* Invalid parameter(s) */
#define SBI_ERR_DENIED -4            /* Denied or not allowed */
#define SBI_ERR_INVALID_ADDRESS -5   /* Invalid address(s) */
#define SBI_ERR_ALREADY_AVAILABLE -6 /* Already available */
#define SBI_ERR_ALREADY_STARTED -7   /* Already started */
#define SBI_ERR_ALREADY_STOPPED -8   /* Already stopped */
#define SBI_ERR_NO_SHMEM -9          /* Shared memory not available */
#define SBI_ERR_INVALID_STATE -10    /* Invalid state */
#define SBI_ERR_BAD_RANGE -11        /* Bad (or invalid) range */
#define SBI_ERR_TIMEOUT -12          /* Failed due to timeout */

struct sbiret {
  long error;
  union {
    long value;
    unsigned long uvalue;
  };
};

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid);

#endif // SBI_H