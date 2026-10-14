/*
#include "altc/altio.h"
#include "altc/string.h"
#include "s3k/s3k.h"
#include <stdint.h>
#include "../app_sections.h"
#include <stddef.h>

#define APP_0_BAK_PMP_SLOT          3
#define APP_0_PMP_SLOT              4

#define LOCAL_APP_0_BAK_BASE_ADDR   UINT64_C(0x80020000)
#define LOCAL_APP_1_BASE_ADDR       UINT64_C(0x80030000)
#define LOCAL_APP_1_SIZE            UINT64_C(0x10000)

extern uint64_t app1_addr;
extern uint64_t uart_addr;

extern uint32_t free_cap_idx;
extern uint32_t free_cap_bak_idx;
extern uint32_t free_cap_pmp_idx;
extern uint32_t free_reset_cap_index;

void dynamic_move_pmp_access(uint32_t src_pid, uint32_t dst_pid, uint32_t src_idx, uint32_t dst_idx, uint32_t dst_pmp_slot);

*/