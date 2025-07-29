#include <malloc.h>
#include <windows.h>

#include "core/logger.h"
#include "platform.h"

static f64 clock_frequency;
static LARGE_INTEGER start_time;

static input_code input_translation[254];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w_Param, LPARAM l_Param);
void input_translation_array_init();

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

    // Input translation
    input_translation_array_init();

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

void input_translation_array_init() {
    for (u32 i = 0; i < 254; i++) {
        input_translation[i] = INPUT_CODE_COUNT;
    }

    input_translation[VK_LBUTTON] = M_LBUTTON;
    input_translation[VK_RBUTTON] = M_RBUTTON;
    input_translation[VK_MBUTTON] = M_MBUTTON;
    input_translation[VK_XBUTTON1] = M_4BUTTON;
    input_translation[VK_XBUTTON2] = M_5BUTTON;
    input_translation[VK_BACK] = K_BACK,
    input_translation[VK_TAB] = K_TAB,
    input_translation[VK_RETURN] = K_ENTER;
    input_translation[VK_PAUSE] = K_PAUSE;
    input_translation[VK_CAPITAL] = K_CAPITAL;
    input_translation[VK_ESCAPE] = K_ESCAPE;
    input_translation[VK_SPACE] = K_SPACE;
    input_translation[VK_PRIOR] = K_PAGE_UP;
    input_translation[VK_NEXT] = K_PAGE_DOWN;
    input_translation[VK_END] = K_END;
    input_translation[VK_HOME] = K_HOME;
    input_translation[VK_LEFT] = K_LEFT;
    input_translation[VK_UP] = K_UP;
    input_translation[VK_RIGHT] = K_RIGHT;
    input_translation[VK_DOWN] = K_DOWN;
    input_translation[VK_INSERT] = K_INSERT;
    input_translation[VK_DELETE] = K_DELETE;
    input_translation[VK_PRINT] = K_PRINT;
    input_translation['0'] = K_0;
    input_translation['1'] = K_1;
    input_translation['2'] = K_2;
    input_translation['3'] = K_3;
    input_translation['4'] = K_4;
    input_translation['5'] = K_5;
    input_translation['6'] = K_6;
    input_translation['7'] = K_7;
    input_translation['8'] = K_8;
    input_translation['9'] = K_9;
    input_translation['A'] = K_A;
    input_translation['B'] = K_B;
    input_translation['C'] = K_C;
    input_translation['D'] = K_D;
    input_translation['E'] = K_E;
    input_translation['F'] = K_F;
    input_translation['G'] = K_G;
    input_translation['H'] = K_H;
    input_translation['I'] = K_I;
    input_translation['J'] = K_J;
    input_translation['K'] = K_K;
    input_translation['L'] = K_L;
    input_translation['M'] = K_M;
    input_translation['N'] = K_N;
    input_translation['O'] = K_O;
    input_translation['P'] = K_P;
    input_translation['Q'] = K_Q;
    input_translation['R'] = K_R;
    input_translation['S'] = K_S;
    input_translation['T'] = K_T;
    input_translation['U'] = K_U;
    input_translation['V'] = K_V;
    input_translation['W'] = K_W;
    input_translation['X'] = K_X;
    input_translation['Y'] = K_Y;
    input_translation['Z'] = K_Z;
    input_translation[VK_LWIN] = K_LWIN;
    input_translation[VK_RWIN] = K_RWIN;
    input_translation[VK_NUMPAD0] = K_NUM0;
    input_translation[VK_NUMPAD1] = K_NUM1;
    input_translation[VK_NUMPAD2] = K_NUM2;
    input_translation[VK_NUMPAD3] = K_NUM3;
    input_translation[VK_NUMPAD4] = K_NUM4;
    input_translation[VK_NUMPAD5] = K_NUM5;
    input_translation[VK_NUMPAD6] = K_NUM6;
    input_translation[VK_NUMPAD7] = K_NUM7;
    input_translation[VK_NUMPAD8] = K_NUM8;
    input_translation[VK_NUMPAD9] = K_NUM9;
    input_translation[VK_MULTIPLY] = K_MULTIPLY;
    input_translation[VK_ADD] = K_ADD;
    input_translation[VK_SUBTRACT] = K_SUBTRACT;
    input_translation[VK_DECIMAL] = K_DECIMAL;
    input_translation[VK_DIVIDE] = K_DIVIDE;
    input_translation[VK_SEPARATOR] = K_NUM_ENTER;
    input_translation[VK_F1] = K_F1;
    input_translation[VK_F2] = K_F2;
    input_translation[VK_F3] = K_F3;
    input_translation[VK_F4] = K_F4;
    input_translation[VK_F5] = K_F5;
    input_translation[VK_F6] = K_F6;
    input_translation[VK_F7] = K_F7;
    input_translation[VK_F8] = K_F8;
    input_translation[VK_F9] = K_F9;
    input_translation[VK_F10] = K_F10;
    input_translation[VK_F11] = K_F11;
    input_translation[VK_F12] = K_F12;
    input_translation[VK_F13] = K_F13;
    input_translation[VK_F14] = K_F14;
    input_translation[VK_F15] = K_F15;
    input_translation[VK_F16] = K_F16;
    input_translation[VK_F17] = K_F17;
    input_translation[VK_F18] = K_F18;
    input_translation[VK_F19] = K_F19;
    input_translation[VK_F20] = K_F20;
    input_translation[VK_F21] = K_F21;
    input_translation[VK_F22] = K_F22;
    input_translation[VK_F23] = K_F23;
    input_translation[VK_F24] = K_F24;
    input_translation[VK_NUMLOCK] = K_NUMLOCK;
    input_translation[VK_SCROLL] = K_SCROLLLOCK;
    input_translation[VK_LSHIFT] = K_LSHIFT;
    input_translation[VK_RSHIFT] = K_RSHIFT;
    input_translation[VK_LCONTROL] = K_LCONTROL;
    input_translation[VK_RCONTROL] = K_RCONTROL;
    input_translation[VK_LMENU] = K_LMENU;
    input_translation[VK_RMENU] = K_RMENU;
    input_translation[VK_VOLUME_MUTE] = K_VOLUME_MUTE;
    input_translation[VK_VOLUME_DOWN] = K_VOLUME_DOWN;
    input_translation[VK_VOLUME_UP] = K_VOLUME_UP;
    input_translation[VK_MEDIA_NEXT_TRACK] = K_MEDIA_NEXT;
    input_translation[VK_MEDIA_PREV_TRACK] = K_MEDIA_PREV;
    input_translation[VK_MEDIA_STOP] = K_MEDIA_STOP;
    input_translation[VK_MEDIA_PLAY_PAUSE] = K_MEDIA_PLAY_PAUSE;
    input_translation[VK_OEM_3] = K_GRAVE_TILDE;
    input_translation[VK_OEM_MINUS] = K_DASH_UNDERSCORE;
    input_translation[VK_OEM_PLUS] = K_EQUALS_PLUS;
    input_translation[VK_OEM_4] = K_LBRACKET;
    input_translation[VK_OEM_6] = K_RBRACKET;
    input_translation[VK_OEM_5] = K_BACKSLASH_PIPE;
    input_translation[VK_OEM_1] = K_SEMICOLON_COLON;
    input_translation[VK_OEM_7] = K_APOSTROPHE_DBLQUOTE;
    input_translation[VK_OEM_COMMA] = K_COMMA_LESSTHAN;
    input_translation[VK_OEM_PERIOD] = K_PERIOD_GREATERTHAN;
    input_translation[VK_OEM_2] = K_FORWARDSLASH_QUESTION;
}