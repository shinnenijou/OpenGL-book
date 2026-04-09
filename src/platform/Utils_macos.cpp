//
// macOS-specific implementations for Utils
// Created by YuxinLiu on 2026/4/9.
//

#include "Utils_platform.h"
#include <mach-o/dyld.h>
#include <cstring>

std::filesystem::path GetExecutablePathImpl() {
    char buf[1024];
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) == 0) {
        return std::filesystem::path{buf, buf + std::strlen(buf)};
    }
    return {};
}
