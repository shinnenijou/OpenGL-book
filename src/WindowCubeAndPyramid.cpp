#include "opengl.h"
#include "Utils.h"
#include "debug.h"
#include "WindowCubeAndPyramid.h"


void WindowCubeAndPyramid::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-4.5.glsl", "res/shader/fragShader-4.5.glsl");

    camara_ = {0.0f, 0.0f, 8.0f};
    cube_loc_ = {0.0f, -2.0f, 0.0f};
    pyramid_loc_ = {2.0f, 2.0f, 0.0f};

    // Create and bind vao
    glGenVertexArrays(VAO_NUM, vao_);
    glBindVertexArray(vao_[0]);

    // Create vbo
    glGenBuffers(VBO_NUM, vbo_);

    SetupCubeVertices();
    SetupPyramidVertices();
}

void WindowCubeAndPyramid::Display(const double current_time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);

    // fetch the reference to uniform variables
    const int mvLoc = glGetUniformLocation(rendering_program_, "mv_matrix");
    const int projLoc = glGetUniformLocation(rendering_program_, "proj_matrix");

    // create perspective matrix
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    const auto pMat = glm::perspective(FOVY, aspect, 0.1f, 1000.0f);

    // create a common view matrix for two objects
    const auto vMat = translate(glm::mat4(1.0f), - camara_);

    // create other matrix repectively
    // for the cube
    auto mMat = glm::translate(glm::mat4(1.0f), cube_loc_);
    auto mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_CUBE]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // for pyramid
    mMat = glm::translate(glm::mat4(1.0f), pyramid_loc_);
    mvMat = vMat * mMat;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_PYRAMID]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);
}

void WindowCubeAndPyramid::SetupCubeVertices()
{
    constexpr float vertex_positions[] = {
       -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
        1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,
    };

    // bind vertex postions to the first vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_CUBE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}

void WindowCubeAndPyramid::SetupPyramidVertices() {
    constexpr float vertex_positions[] = {
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 前面
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 右面
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 后面
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 左面
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // 底面左前
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // 底面右后
    };

    // bind vertex postions to the second vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_PYRAMID]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}
