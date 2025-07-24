#include <malloc.h>
#include <windows.h>

#include "core/logger.h"
#include "platform.h"

static f64 clock_frequency;
static LARGE_INTEGER start_time;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_Param, LPARAM l_Param);

b8 platform_init() {
    WNDCLASSEXW wc;
    HWND handle;
    HINSTANCE hInstance = GetModuleHandleW(NULL);

    // Setup class
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"main_window_class";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // Register class
    if (!RegisterClassExW(&wc)) {
        LOG_FATAL("Window class registration failed.")
        return FALSE;
    }

    // Create Window from registered class
    handle = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"main_window_class",
        L"Morrow Engine",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if (handle == NULL) {
        LOG_FATAL("Window creation failed.")
        return FALSE;
    }

    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);

    // Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);

    return TRUE;
}

void platform_shutdown() {
}

b8 platform_pump_messages() {
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

f64 platform_get_absolute_time() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (f64)now.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

void platform_console_write(const char* message, u8 color) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG
    static u8 levels[5] = {64, 4, 6, 2, 1};
    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char* message, u8 color) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG
    static u8 levels[5] = {64, 4, 6, 2, 1};
    SetConsoleTextAttribute(console_handle, levels[color]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

void* platform_allocate(u64 size, u16 alignment) {
    void* ptr;
    if (alignment <= 8) {
        ptr = malloc(size);
    } else {
        ptr = _aligned_malloc(size, alignment);
    }
    return ptr;
}

void platform_free(void* block, u16 alignment) {
    if (alignment <= 8) {
        free(block);
    } else {
        _aligned_free(block);
    }
}

void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void* platform_move_memory(void* dest, const void* source, u64 size) {
    return memmove(dest, source, size);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_Param, LPARAM l_Param) {
    switch (msg) {
        default:
            return DefWindowProc(hwnd, msg, w_Param, l_Param);
    }
    return 0;
}