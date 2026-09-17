#pragma once

#include "cap_types.h"

#include <stdbool.h>
#include <stdint.h>

void shadow_stack_init(void);

bool shadow_stack_push(pid_t pid, uint64_t ra);

bool shadow_stack_pop(pid_t pid, uint64_t ra);
