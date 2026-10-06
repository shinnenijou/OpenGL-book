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

    SetupVertices(VERTICES_CUBE_DEF, sizeof(VERTICES_CUBE_DEF), 0);
    SetupVertices(VERTICES_PYRAMID_DEF, sizeof(VERTICES_PYRAMID_DEF), 1);
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

void WindowCubeAndPyramid::SetupVertices(const float points[], int size, int index) {
    CHECK_ARRAY_INDEX_NORET(vbo_, index);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[index]);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
}


