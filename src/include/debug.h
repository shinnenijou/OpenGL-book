//
// Created by YuxinLiu on 2025/5/19.
//

#ifndef DEBUG_H
#define DEBUG_H

#include "opengl.h"
#include <iostream>

void printShaderLog(GLuint shader);
void printProgramLog(GLuint program);
bool checkOpenGLError();

#define GLCall(expr)    expr;   \
if (checkOpenGLError()) exit(1)

#endif //DEBUG_H
