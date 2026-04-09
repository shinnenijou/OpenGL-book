//
// Platform-specific implementations for Utils
// Created by YuxinLiu on 2026/4/9.
//

#ifndef UTILS_PLATFORM_H
#define UTILS_PLATFORM_H

#include <filesystem>

// Platform-specific implementation of GetExecutablePath
// Each platform file (Utils_macos.cpp, Utils_windows.cpp, Utils_linux.cpp)
// must provide this function.
std::filesystem::path GetExecutablePath();

#endif //UTILS_PLATFORM_H
