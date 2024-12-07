#ifndef WINAPI_H
#define WINAPI_H

#include "windef.h"

typedef struct {
    char title[256];
    RECT rect;
    int layer;
} DesktopWindow;

typedef struct {
    DesktopWindow* windows;
    int* count;
} EnumWindowsData;

void InitWindowSettings();
RECT GetWorkArea();
void GetWindowsByLayer(DesktopWindow* windows, int* count);

#endif // WINAPI_H