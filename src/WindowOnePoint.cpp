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

void WindowOnePoint::Display(const double current_time)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);
    const auto factor = static_cast<float>(glm::abs(glm::sin(current_time)));
    glPointSize(30.0f * factor);
    glDrawArrays(GL_POINTS, 0, 1);
}


