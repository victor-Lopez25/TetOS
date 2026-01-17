#ifndef _STDBOOL_H
#define _STDBOOL_H

#define __bool_true_false_are_defined 1

/* C23 defines bool, true, false */
#if !defined(__cplusplus) && !(defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L))

#define bool  _Bool
#define false 0
#define true  1

#endif

#endif // _STDBOOL_H