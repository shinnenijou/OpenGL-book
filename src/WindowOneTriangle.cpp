#include "WindowOneTriangle.h"
#include "Utils.h"

void WindowOneTriangle::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-2.5.glsl", "res/shader/fragShader-2.2.glsl");
    glGenVertexArrays(NUM_VAOS, vao_);
    glBindVertexArray(vao_[0]);
}

void WindowOneTriangle::Display(const double current_time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);

    x_ += inc_;
    if (x_ > 1.0f) inc_ = - 0.01f;
    if (x_ < -1.0f) inc_ = 0.01f;
    const int offsetLoc = glGetUniformLocation(rendering_program_, "offset");
    glProgramUniform1f(rendering_program_, offsetLoc, x_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
