#pragma once

#include "darray_internal.h"
#include "defines.h"

#define darray_create(type) _darray_create(INITIAL_DARRAY_COUNT, sizeof(type))
#define darray_reserve(type, capacity) _darray_create(capacity, sizeof(type))
#define darray_destroy(array) _darray_destroy(array);
#define darray_push(array, value)           \
    {                                       \
        typeof(value) temp = value;         \
        array = _darray_push(array, &temp); \
    }
#define darray_pop(array, dest) _darray_pop(array, dest)
#define darray_insert(array, value, index)           \
    {                                                \
        typeof(value) temp = value;                  \
        array = _darray_insert(array, &temp, index); \
    }
#define darray_remove(array, dest, index) _darray_remove(array, dest, index)
#define darray_clear(array) DARRAY_SET_FIELD(array, DARRAY_LENGTH, 0)
#define darray_capacity(array) DARRAY_GET_FIELD(array, DARRAY_CAPACITY)
#define darray_length(array) DARRAY_GET_FIELD(array, DARRAY_LENGTH)
#define darray_stride(array) DARRAY_GET_FIELD(array, DARRAY_STRIDE)
#define darray_length_set(array, new_length) DARRAY_SET_FIELD(array, DARRAY_LENGTH, new_length)
