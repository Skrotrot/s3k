// source: https://balau82.wordpress.com/2010/10/06/trace-and-profile-function-calls-with-gcc/
#include "altc/altio.h"

__attribute__((no_instrument_function))
void __cyg_profile_func_enter (void *func, void *caller) {
    alt_printf("e - func: %X, caller: %X\n", func, caller);
}
__attribute__((no_instrument_function))
void __cyg_profile_func_exit (void *func, void *caller) {
    alt_printf("x - func: %X, caller: %X\n", func, caller);
}
