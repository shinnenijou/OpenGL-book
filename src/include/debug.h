//
// Created by YuxinLiu on 2025/5/19.
//

#ifndef DEBUG_H
#define DEBUG_H

#include "opengl.h"
#include <iostream>
#include <string>

void printShaderLog(GLuint shader, const std::string &func, int line);
void printProgramLog(GLuint program, const std::string &func, int line);
bool checkOpenGLError();

#define GLCall(expr)    expr;   \
if (checkOpenGLError()) exit(1)

#define SHADER_LOG(shader) printShaderLog((shader), __FUNCTION__, __LINE__)
#define PROGRAM_LOG(program) printProgramLog(program, __FUNCTION__, __LINE__)

#endif //DEBUG_H
