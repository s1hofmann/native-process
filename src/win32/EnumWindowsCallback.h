#pragma once

#include <Windows.h>
#include <vector>

#include "EnumWindowsParam.h"

BOOL CALLBACK EnumWindowsHandler(HWND hwnd, LPARAM lParam) {
    if (!IsWindowVisible(hwnd)) {
        return TRUE;
    }

    // Retrieve the pointer passed into this callback, and re-'type' it.
    // The only way for a C API to pass arbitrary data is by means of a void*.
    EnumWindowsParam& param = *reinterpret_cast<EnumWindowsParam*>(lParam);

    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    if (processId != param.pid) {
        return TRUE;
    }
    param.windowHandles.push_back(hwnd);

    return TRUE;
}