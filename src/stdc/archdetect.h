#ifndef _ARCHDETECT_H
#define _ARCHDETECT_H

#if defined(__clang__)
# define COMPILER_CLANG 1
# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#  define ARCH_X64 1
# elif defined(i386) || defined(__i386) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# elif defined(__arm__)
#  define ARCH_ARM32 1
# elif defined(__riscv)
#  if defined(__riscv32)
#   define ARCH_RISCV32 1
#  else
#   define ARCH_RISCV64 1
#  endif
# else
#  error architecture not supported yet
# endif
#elif defined(__GNUC__) || defined(__GNUG__)
# define COMPILER_GCC 1
# if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#  define ARCH_X64 1
# elif defined(i386) || defined(__i386) || defined(__i386__)
#  define ARCH_X86 1
# elif defined(__aarch64__)
#  define ARCH_ARM64 1
# elif defined(__arm__)
#  define ARCH_ARM32 1
# elif defined(__riscv)
#  if defined(__riscv32)
#   define ARCH_RISCV32 1
#  else
#   define ARCH_RISCV64 1
#  endif
# else
#  error architecture not supported yet
# endif
#elif defined(_MSC_VER)
# define COMPILER_CL 1
// msvc doesn't support riscv
# if defined(_M_AMD64)
#  define ARCH_X64 1
# elif defined(_M_IX86)
#  define ARCH_X86 1
# elif defined(_M_ARM64)
#  define ARCH_ARM64 1
# elif defined(_M_ARM)
#  define ARCH_ARM32 1
# else
#  error architecture not supported yet
# endif
#else
# error compiler not supported yet
#endif

#if defined(ARCH_X64) || defined(ARCH_RISCV64) || defined(ARCH_ARM64)
# define ARCH_64BIT 1
#elif defined(ARCH_X86) || defined(ARCH_RISCV32) || defined(ARCH_ARM32)
# define ARCH_32BIT 1
#endif

// zeroify
#if !defined(ARCH_32BIT)
#define ARCH_32BIT 0
#endif
#if !defined(ARCH_64BIT)
#define ARCH_64BIT 0
#endif
#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif
#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif
#if !defined(ARCH_ARM32)
#define ARCH_ARM32 0
#endif
#if !defined(ARCH_RISCV32)
#define ARCH_RISCV32 0
#endif
#if !defined(ARCH_RISCV64)
#define ARCH_RISCV64 0
#endif
#if !defined(COMPILER_CL)
#define COMPILER_CL 0
#endif
#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif

#if COMPILER_CL
# define COMPILER_NAME "cl"
#elif COMPILER_CLANG
# define COMPILER_NAME "clang"
#elif COMPILER_GCC
# define COMPILER_NAME "gcc"
#else
# error no name for this compiler
#endif

#if ARCH_X86
# define ARCH_NAME "x86"
#elif ARCH_X64
# define ARCH_NAME "x64"
#elif ARCH_ARM64
# define ARCH_NAME "arm64"
#elif ARCH_ARM32
# define ARCH_NAME "arm32"
#elif ARCH_RISCV64
# define ARCH_NAME "rv64"
#elif ARCH_RISCV32
# define ARCH_NAME "rv32"
#else
# error no name for this architecture
#endif

#endif // _ARCHDETECT_H