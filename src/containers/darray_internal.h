#pragma once

#include "defines.h"

typedef enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGTH
} DARRAY_HEADER;

#define DARRAY_GROWTH_FACTOR 2
#define INITIAL_DARRAY_COUNT 2

#define DARRAY_HEADER_SIZE sizeof(u64) * DARRAY_FIELD_LENGTH;
#define DARRAY_HEADER(array) ((u64*)(array) - DARRAY_FIELD_LENGTH)
#define DARRAY_GET_FIELD(array, field) (DARRAY_HEADER(array)[field])
#define DARRAY_SET_FIELD(array, field, value) (DARRAY_HEADER(array)[field] = value)

void* _darray_create(u8 initial_count, u64 element_size);
void _darray_destroy(void* array);

void* _darray_resize(void* array);

void* _darray_push(void* array, const void* value_ptr);
void _darray_pop(void* array, void* dest);

void* _darray_insert(void* array, const void* value_ptr, u64 index);
void _darray_remove(void* array, void* dest, u64 index);