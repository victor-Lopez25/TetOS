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

///////////////////////
// Base Extension
///////////////////////

#ifdef SBI_USE_FUNCTIONS

/* Get SBI specification version */
struct sbiret sbi_get_spec_version(void);
/* Get SBI implementation ID */
struct sbiret sbi_get_impl_id(void);
/* Get SBI implementation version */
struct sbiret sbi_get_impl_version(void);
/* Probe SBI extension (check if it's available) */
struct sbiret sbi_probe_extension(void);
/* get machine vendor ID */
struct sbiret sbi_get_mvendorid(void);
/* get machine architecture ID */
struct sbiret sbi_get_marchid(void);
/* get machine implementation ID */
struct sbiret sbi_get_mimpid(void);

#else

/* Get SBI specification version */
#define sbi_get_spec_version() sbi_call(0, 0, 0, 0, 0, 0, 0x0, 0x10)
/* Get SBI implementation ID */
#define sbi_get_impl_id() sbi_call(0, 0, 0, 0, 0, 0, 1, 0x10)
/* Get SBI implementation version */
#define sbi_get_impl_version() sbi_call(0, 0, 0, 0, 0, 0, 2, 0x10)
/* Probe SBI extension (check if it's available) */
#define sbi_probe_extension(extension_id) sbi_call(extension_id, 0, 0, 0, 0, 0, 3, 0x10)
/* get machine vendor ID */
#define sbi_get_mvendorid() sbi_call(0, 0, 0, 0, 0, 0, 4, 0x10)
/* get machine architecture ID */
#define sbi_get_marchid() sbi_call(0, 0, 0, 0, 0, 0, 5, 0x10)
/* get machine implementation ID */
#define sbi_get_mimpid() sbi_call(0, 0, 0, 0, 0, 0, 6, 0x10)

#endif

/* sbi Debug Console Extension EID */
#define SBI_DBCN_EID 0x4442434E



#endif // SBI_H