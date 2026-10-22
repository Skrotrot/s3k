#pragma once

#include <stdint.h>
#include "altc/altio.h"

#define PTR_KEY 4u

#define PTR_TYPE_ID(ptr) _Generic((ptr), \
    void (*)(void):         1u,          \
    void (*)(int, int):     2u)

#define PTR_TYPE_STRUCT(ptr) _Generic((ptr),  \
    void (*)(void):         (struct function_ptr_1){0}, \
    void (*)(int, int):     (struct function_ptr_2){0})

#define PTR_AUTH_INIT(fun) \
    (__typeof__(PTR_TYPE_STRUCT(&fun))){.callback_fun = fun, .signature = ((((uint64_t)PTR_KEY << 32) | PTR_TYPE_ID(&fun)) ^ (uint64_t)&fun)}

#define PTR_AUTH(ptr,...) \
    if (ptr.signature == ((((uint64_t)PTR_KEY << 32) | PTR_TYPE_ID(ptr.callback_fun)) ^ (uint64_t)ptr.callback_fun)) \
        ptr.callback_fun(__VA_ARGS__); \
    else \
        alt_printf("Function pointer overwrite detected");

struct function_ptr_1 {
    void (*callback_fun)(void);
    uint64_t signature;
};

struct function_ptr_2 {
    void (*callback_fun)(int, int);
    uint64_t signature;
};
