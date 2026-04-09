//
// Created by YuxinLiu on 2025/5/19.
//

#ifndef UTILS_H
#define UTILS_H

#include "opengl.h"
#include <string>
#include <filesystem>

struct Utils {
    using path = std::filesystem::path;
    static auto ReadShaderSource(const path& path) -> std::string;
    static auto CreateShaderProgram(const path& vShader, const path &fShader) -> GLuint;
};



#endif //UTILS_H
