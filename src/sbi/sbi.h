#ifndef SBI_H
#define SBI_H

#define SBI_XLEN_ONES (~(unsigned long)0)

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

struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, 
                       long arg4, long arg5, long fid, long eid);

//////////////////////////////////
// Base Extension
//////////////////////////////////

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

//////////////////////////////////
// Legacy Extensions 0x00 - 0x0F
//////////////////////////////////

/* Blocking */
#define sbi_console_putchar(ch) sbi_call(ch, 0, 0, 0, 0, 0, 0, 0x1)
/* Puts all the harts to shutdown state from supervisor point of view */
#define sbi_shutdown() sbi_call(0, 0, 0, 0, 0, 0, 0, 0x08)

//////////////////////////////////
// Debug Console Extension
//////////////////////////////////

/* sbi Debug Console Extension EID */
#define SBI_DBCN_EID 0x4442434E

/* Write bytes to the debug console from input memory
 * Params:
 *  - num_bytes specifies the number of bytes in the input memory
 *  - physical base address of the input memory split in two XLEN bits wide parameters
 * Returns:
 *  - nº of bytes written in sbiret.value
 *  - possible error codes in sbiret.error
 **/
#define sbi_debug_console_write(num_bytes, base_addr_lo, base_addr_hi) \
  sbi_call(num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0, 0, SBI_DBCN_EID)

//////////////////////////////////
// Performance Monitoring Unit Extension
//////////////////////////////////

#define SBI_PMU_EID 0x504D55

/* event_idx[19:16] = type
 * event_idx[15:0] = code */

/* Hardware general events (type 0) */
#define SBI_PMU_HW_NO_EVENT                0
#define SBI_PMU_HW_CPU_CYCLES              1
#define SBI_PMU_HW_INSTRUCTIONS            2
#define SBI_PMU_HW_CACHE_REFERENCES        3
#define SBI_PMU_HW_CACHE_MISSES            4
#define SBI_PMU_HW_BRANCH_INSTRUCTIONS     5
#define SBI_PMU_HW_BRANCH_MISSES           6
#define SBI_PMU_HW_BUS_CYCLES              7
#define SBI_PMU_HW_STALLED_CYCLES_FRONTEND 8
#define SBI_PMU_HW_STALLED_CYCLES_BACKEND  9
#define SBI_PMU_HW_REF_CPU_CYCLES          10

/* event_idx[15:3] = cache_id
 * event_idx[2:1] = op_id
 * event_idx[0:0] = result_id */

/* Hardware cache events (type 1) */
#define SBI_PMU_HW_CACHE_L1D  0 /* Level1 data cache event */
#define SBI_PMU_HW_CACHE_L1I  1 /* Level1 instruction cache event */
#define SBI_PMU_HW_CACHE_LL   2 /* Last level cache event */
#define SBI_PMU_HW_CACHE_DTLB 3 /* Data TLB event */
#define SBI_PMU_HW_CACHE_ITLB 4 /* Instruction TLB event */
#define SBI_PMU_HW_CACHE_BPU  5 /* Branch predictor unit event */
#define SBI_PMU_HW_CACHE_NODE 6 /* NUMA node cache event */

#define SBI_PMU_HW_CACHE_OP_READ     0 /* Read cache line */
#define SBI_PMU_HW_CACHE_OP_WRITE    1 /* Write cache line */
#define SBI_PMU_HW_CACHE_OP_PREFETCH 2 /* Prefetch cache line */

#define SBI_PMU_HW_CACHE_RESULT_ACCESS 0 /* Cache access */
#define SBI_PMU_HW_CACHE_RESULT_MISS   1 /* Cache miss */

/* Hardware raw events (event_idx.type 2, event_idx.code 0) */

/* Firmware events (type 0xF) */

/* Misaligned load trap event */
#define SBI_PMU_FW_MISALIGNED_LOAD           0
/* Misaligned store trap event */
#define SBI_PMU_FW_MISALIGNED_STORE          1
/* Load access trap event */
#define SBI_PMU_FW_ACCESS_LOAD               2
/* Store access trap event */
#define SBI_PMU_FW_ACCESS_STORE              3
/* Illegal instruction trap event */
#define SBI_PMU_FW_ILLEGAL_INSN              4
/* Set timer event */
#define SBI_PMU_FW_SET_TIMER                 5
/* Sent IPI to other hart event */
#define SBI_PMU_FW_IPI_SENT                  6
/* Received IPI from other hart event */
#define SBI_PMU_FW_IPI_RECEIVED              7
/* Sent FENCE.I request to other hart event */
#define SBI_PMU_FW_FENCE_I_SENT              8
/* Received FENCE.I request from other hart event */
#define SBI_PMU_FW_FENCE_I_RECEIVED          9 
/* Sent SFENCE.VMA request to other hart event */
#define SBI_PMU_FW_SFENCE_VMA_SENT           10
/* Received SFENCE.VMA request from other hart event */
#define SBI_PMU_FW_SFENCE_VMA_RECEIVED       11
/* Sent SFENCE.VMA with ASID request to other hart event */
#define SBI_PMU_FW_SFENCE_VMA_ASID_SENT      12
/* Received SFENCE.VMA with ASID request from other hart event */
#define SBI_PMU_FW_SFENCE_VMA_ASID_RECEIVED  13
/* Sent HFENCE.GVMA request to other hart event */
#define SBI_PMU_FW_HFENCE_GVMA_SENT          14
/* Received HFENCE.GVMA request from other hart event */
#define SBI_PMU_FW_HFENCE_GVMA_RECEIVED      15
/* Sent HFENCE.GVMA with VMID request to other hart event */
#define SBI_PMU_FW_HFENCE_GVMA_VMID_SENT     16
/* Received HFENCE.GVMA with VMID request from other hart event */
#define SBI_PMU_FW_HFENCE_GVMA_VMID_RECEIVED 17
/* Sent HFENCE.VVMA request to other hart event */
#define SBI_PMU_FW_HFENCE_VVMA_SENT          18
/* Received HFENCE.VVMA request from other hart event */
#define SBI_PMU_FW_HFENCE_VVMA_RECEIVED      19
/* Sent HFENCE.VVMA with ASID request to other hart event */
#define SBI_PMU_FW_HFENCE_VVMA_ASID_SENT     20
/* Received HFENCE.VVMA with ASID request from other hart event */
#define SBI_PMU_FW_HFENCE_VVMA_ASID_RECEIVED 21
/* 22 - 255 reserved for future use */
/* 256-65534 SBI implementation specific firmware events */

/* RISC-V platform specific firmware events, where the event_data configuration
 * (or parameter) contains the event encoding */
#define SBI_PMU_FW_PLATFORM                  65535

/* Returns the number of counters (both hardware and firmware) in sbiret.value */
#define sbi_pmu_num_counters() sbi_call(0, 0, 0, 0, 0, 0, 0, SBI_PMU_EID)
/* Returns (in sbiret.value):
 *  counter_info[11:0] = CSR (12bit CSR number)
 *  counter_info[17:12] = Width (One less than number of bits in CSR)
 *  counter_info[__riscv_xlen-2:18] = Reserved for future use
 *  counter_info[__riscv_xlen-1] = Type (0 = hardware, 1 = firmware)
 */
#define sbi_pmu_counter_get_info(counter_idx) \
  sbi_call(counter_idx, 0, 0, 0, 0, 0, 1, SBI_PMU_EID)

// TODO: The rest of the functions

//////////////////////////////////
// System Reset Extension
//////////////////////////////////

#define SBI_SRST_EID 0x53525354

enum {
  SBI_RESET_TYPE_Shutdown = 0,
  SBI_RESET_TYPE_ColdReboot = 1,
  SBI_RESET_TYPE_WarmReboot = 2,
  /* 0x0000'0003 - 0xEFFF'FFFF reserved for future use */
  /* 0xF000'0000 - 0xFFFF'FFFF vendor or platform specific reset type */
};

enum {
  SBI_RESET_REASON_NoReason = 0,
  SBI_RESET_REASON_SystemFailure = 1,
  /* 0x0000'0002 - 0xDFFF'FFFF reserved for future use */
  /* 0xE000'0000 - 0xEFFF'FFFF sbi implementation specific reset reason */
  /* 0xF000'0000 - 0xFFFF'FFFF vendor or platform specific reset reason */
};

#define sbi_system_reset(reset_type, reset_reason) \
  sbi_call(reset_type, reset_reason, 0, 0, 0, 0, 0, SBI_SRST_EID)

//////////////////////////////////
// Time Extension
//////////////////////////////////

#define SBI_TIME_EID 0x54494D45

#if __riscv_xlen == 64
#define sbi_set_timer(stime_value) \
  sbi_call(stime_value, 0, 0, 0, 0, 0, 0, SBI_TIME_EID)
#elif __riscv_xlen == 32
#define sbi_set_timer(stime_value) \
  sbi_call((stime_value) & 0xFFFFFFFF, (stime_value) >> 32, 0, 0, 0, 0, 0, SBI_TIME_EID)
#endif

#endif // SBI_H