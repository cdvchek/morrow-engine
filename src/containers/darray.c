#include "darray.h"

#include "core/memsys.h"
#include "darray_internal.h"

void* _darray_create(u8 initial_count, u64 element_size) {
    // Calculate total allocation
    u64 header_size = DARRAY_HEADER_SIZE;
    u64 remainder = (header_size % element_size);
    u64 padding = remainder == 0 ? 0 : (element_size - remainder);
    u64 array_allocation = (u64)initial_count * element_size;
    u64 total_allocation = padding + header_size + array_allocation;

    // Allocate memory and find the header
    u8* array = (u8*)memsys_allocate(total_allocation, MEMSYS_CODE_DARRAY, element_size);
    array += padding;
    u64* header = (u64*)array;

    // Set the header
    header[DARRAY_CAPACITY] = initial_count;
    header[DARRAY_LENGTH] = 0;
    header[DARRAY_STRIDE] = element_size;

    // Return start of array (right after header)
    return (void*)((u8*)array + header_size);
}

void _darray_destroy(void* array) {
    if (!array) return;

    // Find values from header
    u64 capacity = DARRAY_GET_FIELD(array, DARRAY_CAPACITY);
    u64 stride = DARRAY_GET_FIELD(array, DARRAY_STRIDE);

    // Calculate total allocation
    u64 header_size = DARRAY_HEADER_SIZE;
    u64 array_allocation = capacity * stride;
    u64 remainder = (header_size % stride);
    u64 padding = remainder == 0 ? 0 : (stride - remainder);
    u64 total_allocation = padding + header_size + array_allocation;

    // Find the base ptr
    u8* base = (u8*)array - (header_size + padding);

    // Free the base ptr
    memsys_free(base, total_allocation, MEMSYS_CODE_DARRAY, stride);
}

// Grows the array by a factor of DARRAY_GROW_FACTOR and returns new array
void* _darray_resize(void* array) {
    if (!array) return NULL;

    // Get length, capacity, and stride
    u64 capacity = DARRAY_GET_FIELD(array, DARRAY_CAPACITY);
    u64 length = DARRAY_GET_FIELD(array, DARRAY_LENGTH);
    u64 stride = DARRAY_GET_FIELD(array, DARRAY_STRIDE);

    // Create a new array with bigger capacity based on growth factor
    void* temp = _darray_create(capacity * DARRAY_GROWTH_FACTOR, stride);

    // Copy old array into new array
    memsys_copy(temp, array, length * stride);

    // Set header of new array
    DARRAY_SET_FIELD(temp, DARRAY_LENGTH, length);

    // Destroy old array
    _darray_destroy(array);

    return temp;
}

void* _darray_push(void* array, const void* value_ptr) {
    if (!array) return NULL;

    u64 length = DARRAY_GET_FIELD(array, DARRAY_LENGTH);
    u64 stride = DARRAY_GET_FIELD(array, DARRAY_STRIDE);

    // check if array needs to be bigger
    if (length >= DARRAY_GET_FIELD(array, DARRAY_CAPACITY)) {
        array = _darray_resize(array);
    }

    // insert value at end of array
    u8* insert_addr = (u8*)array + (length * stride);
    memsys_copy(insert_addr, value_ptr, stride);

    // update array length
    DARRAY_SET_FIELD(array, DARRAY_LENGTH, length + 1);

    // return array
    return array;
}

void _darray_pop(void* array, void* dest) {
    if (!array || !dest) return;

    u64 length = DARRAY_GET_FIELD(array, DARRAY_LENGTH);
    u64 stride = DARRAY_GET_FIELD(array, DARRAY_STRIDE);

    // check if empty
    if (length <= 0) return NULL;

    u8* remove_addr = (u8*)array + ((length - 1) * stride);

    // make a void* with value of last element in array
    memsys_copy(dest, remove_addr, stride);

    // edit header
    DARRAY_SET_FIELD(array, DARRAY_LENGTH, length - 1);
}

void* _darray_insert(void* array, const void* value_ptr, u64 index) {
    if (!array) return NULL;

    u64 length = DARRAY_GET_FIELD(array, DARRAY_LENGTH);

    // check if index is in bounds
    if (index > length) return NULL;

    u64 stride = DARRAY_GET_FIELD(array, DARRAY_STRIDE);

    // check if array needs to be resized
    if (length >= DARRAY_GET_FIELD(array, DARRAY_CAPACITY)) {
        array = _darray_resize(array);
    }

    u8* index_addr = (u8*)array + (index * stride);
    u64 bytes_to_move = stride * (length - index);

    if (bytes_to_move) memsys_move(index_addr + stride, index_addr, bytes_to_move);
    memsys_copy(index_addr, value_ptr, stride);

    DARRAY_SET_FIELD(array, DARRAY_LENGTH, length + 1);
    return array;
}

void _darray_remove(void* array, void* dest, u64 index) {
    if (!array || !dest) return;

    u64 length = DARRAY_GET_FIELD(array, DARRAY_LENGTH);

    // check if index is in bounds
    if (index >= length) return;

    u64 stride = DARRAY_GET_FIELD(array, DARRAY_STRIDE);

    u8* index_addr = (u8*)array + (index * stride);
    u64 bytes_to_move = stride * (length - index - 1);

    memsys_copy(dest, index_addr, stride);
    if (bytes_to_move) memsys_move(index_addr, index_addr + stride, bytes_to_move);

    DARRAY_SET_FIELD(array, DARRAY_LENGTH, length - 1);
}