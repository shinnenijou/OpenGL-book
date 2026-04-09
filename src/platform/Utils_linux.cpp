//
// Linux-specific implementations for Utils
// Created by YuxinLiu on 2026/4/9.
//

#include "Utils_platform.h"
#include <unistd.h>
#include <limits.h>

std::filesystem::path GetExecutablePath() {
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        return std::filesystem::path{buf};
    }
    return {};
}
