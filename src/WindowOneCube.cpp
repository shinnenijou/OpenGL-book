#include "opengl.h"
#include "Utils.h"
#include "WindowOneCube.h"


void WindowOneCube::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-4.5.glsl", "res/shader/fragShader-4.5.glsl");

    camara_.x = 0.0f;
    camara_.y = 0.0f;
    camara_.z = 8.0f;

    cube_loc_.x = 0.0f;
    cube_loc_.y = -2.0f;
    cube_loc_.z = 0.0f;

    SetupVertices();
}

void WindowOneCube::Display(const double current_time)
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
    auto pMat = glm::perspective(FOVY, aspect, 0.1f, 1000.0f);

    // create mv matrix
    const auto vMat = translate(glm::mat4(1.0f), - camara_);
    const auto tMat = translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * current_time) * 2.0f, cos(0.52f * current_time) * 2.0f, sin(0.7f * current_time) * 2.0f));
    auto rMat = rotate(glm::mat4(1.0f), 1.75f * static_cast<float>(current_time), glm::vec3(0.0f, 1.0f, 0.0f));
    rMat = rotate(rMat, 1.75f * static_cast<float>(current_time), glm::vec3(1.0f, 0.0f, 0.0f));
    rMat = rotate(rMat, 1.75f * static_cast<float>(current_time), glm::vec3(0.0f, 0.0f, 1.0f));
    const auto mMat = tMat * rMat;
    auto mvMat = vMat * mMat;

    // copy perspective matrix and mv matrix
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // bind vbo to vertex attribute in shaders
    constexpr int location = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(location);

    // setup opengl configure
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void WindowOneCube::SetupVertices()
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

    // Create and bind vao
    glGenVertexArrays(VAO_NUM, vao_);
    glBindVertexArray(vao_[0]);

    // Create
    glGenBuffers(VBO_NUM, vbo_);

    // bind vertex postions to the first vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}
