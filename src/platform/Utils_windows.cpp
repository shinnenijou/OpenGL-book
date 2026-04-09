//
// Windows-specific implementations for Utils
// Created by YuxinLiu on 2026/4/9.
//

#include "Utils_platform.h"
#include <windows.h>

std::filesystem::path GetExecutablePathImpl() {
    char buf[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, buf, MAX_PATH);
    if (len > 0) {
        return std::filesystem::path{buf, buf + len};
    }
    return {};
}
