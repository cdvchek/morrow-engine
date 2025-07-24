#pragma once

#include "core/logger.h"
#include "defines.h"

typedef enum memsys_code {
    MEMSYS_CODE_DARRAY,
    MEMSYS_MAX_CODES
} memsys_code;

void* memsys_allocate(u64 size, memsys_code code, u16 alignment);
void memsys_free(void* block, u64 size, memsys_code code, u16 alignment);
void* memsys_zero(void* block, u64 size);
void* memsys_copy(void* dest, const void* source, u64 size);
void* memsys_set(void* dest, i32 value, u64 size);
void* memsys_move(void* dest, const void* source, u64 size);