#pragma once

#include <stdint.h>
#include "altc/altio.h"

#define PTR_KEY 4ULL

// Assign id to each function pointer type.
#define PTR_TYPE_ID(ptr) _Generic((ptr), \
    void (*)(void):         1ULL,          \
    void (*)(int, int):     2ULL)

// Used for detecting struct type during authentication.
#define PTR_TYPE_STRUCT(ptr) _Generic((ptr),  \
    void (*)(void):         (struct function_ptr_1){0}, \
    void (*)(int, int):     (struct function_ptr_2){0})

#define PTR_AUTH_INIT(fun) \
    (__typeof__(PTR_TYPE_STRUCT(&fun))){.callback_fun = fun, .signature = (PTR_KEY ^ PTR_TYPE_ID(&fun) ^ (uint64_t)&fun)}

#define PTR_AUTH(ptr,...) \
    if (ptr.signature == (PTR_KEY ^ PTR_TYPE_ID(ptr.callback_fun) ^ (uint64_t)ptr.callback_fun)) \
        ptr.callback_fun(__VA_ARGS__); \
    else \
        alt_printf("Function pointer overwrite detected\n");

// Declare function pointers with type and signature.
struct function_ptr_1 {
    void (*callback_fun)(void);
    uint64_t signature;
};

struct function_ptr_2 {
    void (*callback_fun)(int, int);
    uint64_t signature;
};
