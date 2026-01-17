#ifndef _STDDEF_H
#define _STDDEF_H

#define offsetof(type, member) __builtin_offsetof(type, member)

typedef __SIZE_TYPE__ size_t;
typedef __PTRDIFF_TYPE__ ptrdiff_t;

// max_align_t ?

#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
typedef typeof(nullptr) nullptr_t; /* Since C23 */
#define NULL nullptr
#elif defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void*)0)
#endif

#endif // _STDDEF_H