#pragma once

#include "s3k/s3k.h"

#define CFI_ENTER() \
    s3k_msg_t msg = {0}; \
    void *__cfi_ra; \
    __asm__ volatile ("mv %0, ra" : "=r"(__cfi_ra)); \
    msg.data[0] = (uint64_t)__cfi_ra; \
    msg.data[1] = (uint64_t)1; /* msg.data[1] set to 1 in order to distinguish between Enter or Exit. */ \
    s3k_sock_sendrecv(APP_1_CAP_SOCKET, &msg);

#define CFI_RETURN(...) \
    do { \
        s3k_msg_t msg = {0}; \
        void *__cfi_ra_now; \
        __asm__ volatile ("ld %0, -8(s0)" : "=r"(__cfi_ra_now)); \
        msg.data[0] = (uint64_t)__cfi_ra_now; \
        msg.data[1] = (uint64_t)0; \
        s3k_sock_sendrecv(APP_1_CAP_SOCKET, &msg); \
        return __VA_ARGS__; \
    } while (0)
