#ifndef _CALLINSTACK_CONFIG_H_
#define _CALLINSTACK_CONFIG_H_

//Using ENABLE_RIGHT_VALUE_REFERENCE if your GCC version and compiler option supports right value reference!
//#define ENABLE_RIGHT_VALUE_REFERENCE

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if defined __GNUC__

#define DUMMY_RETURN(r_type) return r_type(0);

//Since GCC 6.1.0, -std=c++14 is default
#if (GCC_VERSION >= 60100) && !(defined ENABLE_RIGHT_VALUE_REFERENCE)
#define ENABLE_RIGHT_VALUE_REFERENCE
#endif // GCC_VERSION

#if GCC_VERSION >= 40000
#define DLL_LOCAL   __attribute__((visibility("internal")))
#else
#define DLL_LOCAL
#endif // GCC_VERSION

#define FORCE_NOINLINE __attribute__ ((noinline))


#if GCC_VERSION >= 40400
#define FORCE_OPTIMIZATION(x)   __attribute__((optimize(#x)))
#define ENABLE_PUSH_PRAGMA
#else
#define FORCE_OPTIMIZATION(x)
#endif // GCC_VERSION


#if (GCC_VERSION < 40400) && !(defined __OPTIMIZE__)
#define ALWAYS_HAS_FRAME_POINTER
#endif // __OPTIMIZE__

#if (GCC_VERSION < 40600)
//Just avoid warning (visit pointer of vtable breaks strict-aliasing rules in the view of old GCC version) for -fstrict-aliasing or -O2.
//You can undef the macro to turn on the optimization if you do not care the warning.
#define DISABLE_VTABLE_EXPANSION
#endif

#endif //__GNUC__


#include "call_in_stack_x64_sysv.h"
#include "call_in_stack_x86_sysv.h"

#endif
