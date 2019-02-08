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

    std::cout << "EnumWindows" << std::endl;

    TCHAR title[512];
    GetWindowText(hwnd, title, sizeof(title));
    std::cout << title << std::endl;

    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    param.windowHandles->push_back(hwnd);
    std::cout << "Enum id: " << processId << " param id: " << param.pid << std::endl;
    if (processId != param.pid) {
        return TRUE;
    }
    param.windowHandles->push_back(hwnd);

    return TRUE;
}