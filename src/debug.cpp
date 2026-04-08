#include "debug.h"

#include <cstdlib>



void printShaderLog(GLuint shader, const std::string &func, int line)
{
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        int wn = 0;
        const auto log = new char[len];
        glGetShaderInfoLog(shader, len, &wn, log);
        std::cout << "[" << func << ":" << line << "]" <<  "Shader Info Log: " << log << std::endl;
        delete[] log;
    }
}

void printProgramLog(GLuint program, const std::string &func, int line)
{
    int len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        int wn = 0;
        const auto log = new char[len];
        glGetProgramInfoLog(program, len, &wn, log);
        std::cout << "[" << func << ":" << line << "]" <<  "Program Info Log: " << log << std::endl;
        delete[] log;
    }
}

bool checkOpenGLError()
{
    bool foundError = false;

    for (auto err = glGetError(); err != GL_NO_ERROR; err = glGetError())
    {
        foundError = true;
        std::cerr << "OpenGL Error: " << err << std::endl;
    }

    return foundError;
}