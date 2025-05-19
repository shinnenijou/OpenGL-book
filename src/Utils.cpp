//
// Created by YuxinLiu on 2025/5/19.
//

#include "Utils.h"
#include "debug.h"

#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

const filesystem::path Utils::PROJ_ROOT {"./"};

auto Utils::ReadShaderSource(const std::filesystem::path& path) -> string
{
    string content;
    ifstream fs(PROJ_ROOT / path, ios::in);
    char ch;
    while (fs.get(ch))
        content.push_back(ch);
    return content;
}

auto Utils::CreateShaderProgram(const path& vShader, const path& fShader) -> GLuint
{
    const auto vshaderStr = ReadShaderSource(vShader);
    const auto vshaderSource = vshaderStr.c_str();
    const auto fshaderStr = ReadShaderSource(fShader);
    const auto fshaderSource = fshaderStr.c_str();

    const GLuint vShaderD = GLCall(glCreateShader(GL_VERTEX_SHADER));
    const GLuint fShaderD = GLCall(glCreateShader(GL_FRAGMENT_SHADER));

    GLCall(glShaderSource(vShaderD, 1, &vshaderSource, nullptr));
    glShaderSource(fShaderD, 1, &fshaderSource, nullptr);
    glCompileShader(vShaderD);
    glCompileShader(fShaderD);

    const GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShaderD);
    glAttachShader(vfProgram, fShaderD);
    glLinkProgram(vfProgram);

    return vfProgram;
}