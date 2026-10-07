#ifndef UTILS_H
#define UTILS_H

#include "opengl.h"
#include <string>
#include <filesystem>

struct Utils {
    using path = std::filesystem::path;
    static auto GetRootPath() -> const path&;
    static auto ReadShaderSource(const path& path) -> std::string;
    static auto CreateShaderProgram(const path& vShader, const path &fShader) -> GLuint;

    /**
     * @warning 必须在GLEW初始化以后才能调用, 否则会SEGSIG. 正常应该只在GLFW创建窗口后初始化/帧更新阶段才能使用
     */
    static auto loadTexture(const path& texImagePath) -> GLuint;
    static auto toRadian(double degree) ->float { return degree * M_PI / 180.0f; }
};



#endif //UTILS_H
