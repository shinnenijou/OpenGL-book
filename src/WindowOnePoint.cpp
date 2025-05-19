#include "WindowOnePoint.h"
#include "debug.h"

void WindowOnePoint::Init()
{
    WindowBase::Init();
    rendering_program_ = CreateShaderProgram();
    glGenVertexArrays(NUM_VAOS, vao);
    glBindVertexArray(vao[0]);
}

void WindowOnePoint::Display(double current_time)
{
    glUseProgram(rendering_program_);
    glPointSize(30.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

GLuint WindowOnePoint::CreateShaderProgram()
{
    const auto vshaderSource =
        "#version 410\n"
        "void main(void)\n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const auto fshaderSource =
        "#version 410\n"
        "out vec4 color;\n"
        "void main(void)\n"
        "{ if (gl_FragCoord.x < 295) color = vec4(1.0, 0.0, 0.0, 1.0);"
        "  else color = vec4(0.0, 0.0, 1.0, 1.0); "
        "}";

    const GLuint vShader = GLCall(glCreateShader(GL_VERTEX_SHADER));
    const GLuint fShader = GLCall(glCreateShader(GL_FRAGMENT_SHADER));

    GLCall(glShaderSource(vShader, 1, &vshaderSource, nullptr));
    glShaderSource(fShader, 1, &fshaderSource, nullptr);
    glCompileShader(vShader);
    glCompileShader(fShader);

    const GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    return vfProgram;
}
