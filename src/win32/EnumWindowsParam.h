#pragma once

#include <Windows.h>
#include <vector>

struct EnumWindowsParam {
    DWORD pid;
    std::vector<HWND> *windowHandles;
};