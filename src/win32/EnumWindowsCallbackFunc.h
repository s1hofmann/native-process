#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>

#include "EnumWindowsParam.h"

BOOL CALLBACK EnumWindowsCallbackFunc(HWND hwnd, LPARAM lParam) {
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    EnumWindowsParam& param = *reinterpret_cast<EnumWindowsParam*>(lParam);

    TCHAR title[512];
    GetWindowText(hwnd, title, sizeof(title));

    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId != param.pid) {
        return TRUE;
    } else {
        param.windowHandles->push_back(hwnd);
    }

    return TRUE;
}