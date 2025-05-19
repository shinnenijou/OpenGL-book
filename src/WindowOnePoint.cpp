#include "WindowOnePoint.h"
#include "debug.h"
#include "Utils.h"

void WindowOnePoint::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-2.2.glsl", "res/shader/fragShader-2.2.glsl");
    glGenVertexArrays(NUM_VAOS, vao);
    glBindVertexArray(vao[0]);
}

void WindowOnePoint::Display(double current_time)
{
    glUseProgram(rendering_program_);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}


