#include "memsys.h"

#include "platform/platform.h"

u64 memsys_code_allocations[MEMSYS_MAX_CODES] = {0};

void* memsys_allocate(u64 size, memsys_code code, b8 aligned, u16 alignment) {
    void* ptr = platform_allocate(size, aligned, alignment);
    memsys_code_allocations[code] += size;
    return ptr;
}

void memsys_free(void* block, u64 size, memsys_code code, b8 aligned) {
    platform_free(block, aligned);
    memsys_code_allocations[code] -= size;
}

void* memsys_zero(void* block, u64 size) {
    return platform_zero_memory(block, size);
}

void* memsys_copy(void* dest, const void* source, u64 size) {
    return platform_copy_memory(dest, source, size);
}

void* memsys_set(void* dest, i32 value, u64 size) {
    return platform_set_memory(dest, value, size);
}

void* memsys_move(void* dest, const void* source, u64 size) {
    return platform_move_memory(dest, source, size);
}