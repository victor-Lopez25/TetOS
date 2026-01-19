#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

#include "stdc/stdbool.h"
#include "sbi.h"

// physical address is 34-40 bits in Sv32
typedef uint64_t paddr_t;
typedef size_t vaddr_t;

#endif // KERNEL_H