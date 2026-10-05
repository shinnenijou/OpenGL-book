//
// Created by YuxinLiu on 2025/5/19.
//

#include "Utils.h"
#include "debug.h"
#include "platform/Utils_platform.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <SOIL2.h>

using namespace std;


auto Utils::GetRootPath() -> const path & {
    static path base = GetExecutablePath().parent_path().parent_path();
    return base;
}

auto Utils::ReadShaderSource(const path& relativePath) -> string
{
    static path base = GetRootPath();
    string content;
    ifstream fs(base / relativePath, ios::in);
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

    const GLuint vShaderD =glCreateShader(GL_VERTEX_SHADER);
    const GLuint fShaderD =glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShaderD, 1, &vshaderSource, nullptr);
    glShaderSource(fShaderD, 1, &fshaderSource, nullptr);
    glCompileShader(vShaderD);
    SHADER_LOG(vShaderD);
    glCompileShader(fShaderD);
    SHADER_LOG(fShaderD);

    const GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShaderD);
    PROGRAM_LOG(vfProgram);
    glAttachShader(vfProgram, fShaderD);
    PROGRAM_LOG(vfProgram);
    glLinkProgram(vfProgram);
    PROGRAM_LOG(vfProgram);

    return vfProgram;
}

auto Utils::loadTexture(const path& texImagePath) -> GLuint {

    LOG_INFO("load: %s", (GetRootPath() / texImagePath).c_str());
    const GLuint textureID = SOIL_load_OGL_texture(
        (GetRootPath() / texImagePath).c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (textureID == 0)
    {
        LOG_ERROR("could not find texture file: %s", texImagePath.c_str());
    }

    return textureID;
}