#include <windows.h>
#include <dwmapi.h>
#include <stdbool.h>
#include <stdio.h>
#include "winapi.h"


HWND WindowHandle;

void InitWindowSettings()
{
    WindowHandle = GetActiveWindow();

    LONG_PTR exStyle = GetWindowLongPtr(WindowHandle, GWL_EXSTYLE);
    SetWindowLongPtr(WindowHandle, GWL_EXSTYLE, exStyle | WS_EX_NOACTIVATE | WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST);

    LONG windowStyles = GetWindowLong(WindowHandle, GWL_STYLE);
    SetWindowLong(WindowHandle, GWL_STYLE, windowStyles & ~WS_OVERLAPPEDWINDOW);

    SetLayeredWindowAttributes(WindowHandle, RGB(0, 0, 0), 0, LWA_COLORKEY);

    int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    SetWindowPos(WindowHandle, HWND_TOPMOST, 0, 0, screenWidth, screenHeight - 1, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
}

// Function to get the work area of the screen
RECT GetWorkArea()
{
    RECT WorkAreaRect;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkAreaRect, 0);
    return WorkAreaRect;
}

// Function to get the window title
void GetWindowTitle(HWND hwnd, char* title, int size) {
    if (!GetWindowTextA(hwnd, title, size)) {
        strncpy(title, "Unknown", size - 1);
        title[size - 1] = '\0';
    }
}

// Function to check if a window is cloaked
BOOL IsWindowCloaked(HWND hwnd) {
    BOOL isCloaked = FALSE;
    if (FAILED(DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, sizeof(isCloaked)))) {
        return FALSE;
    }
    return isCloaked;
}

// Function to check if a window is visible on the screen
BOOL IsWindowVisibleOnScreen(HWND hwnd) {
    return IsWindowVisible(hwnd) && !IsWindowCloaked(hwnd) && !IsIconic(hwnd);
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    EnumWindowsData* data = (EnumWindowsData*)lParam;
    DesktopWindow* windows = data->windows;
    int* count = data->count;

    GetWindowTitle(hwnd, windows[*count].title, sizeof(windows[*count].title));

    if (hwnd == WindowHandle || !IsWindowVisibleOnScreen(hwnd) || IsIconic(hwnd) || !IsWindow(hwnd) || !IsWindowVisible(hwnd) || !IsWindowEnabled(hwnd)) {
        return TRUE;
    }

    // Additional checks to filter out unwanted windows
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
    if (style & WS_DISABLED || style & WS_POPUP || style & WS_CHILD) {
        return TRUE;
    }

    RECT WindowRect;
    GetWindowRect(hwnd, &WindowRect);

    windows[*count].rect = WindowRect;
    windows[*count].layer = *count;

    (*count)++;

    return TRUE;
}

void GetWindowsByLayer(DesktopWindow* windows, int* count) {
    EnumWindowsData data = { windows, count };
    *count = 0;
    EnumWindows(EnumWindowsProc, (LPARAM)&data);
}