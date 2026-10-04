#include "opengl.h"
#include "Utils.h"
#include "WindowManyCubes.h"


void WindowManyCubes::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-4.6.glsl", "res/shader/fragShader-4.5.glsl");

    camara_.x = 0.0f;
    camara_.y = 0.0f;
    camara_.z = 8.0f;

    cube_loc_.x = 0.0f;
    cube_loc_.y = -2.0f;
    cube_loc_.z = 0.0f;

    SetupVertices();
}

void WindowManyCubes::Display(const double current_time)
{
    DisplayByInstancing(current_time, 100000);
}

void WindowManyCubes::SetupVertices()
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

// 10K cubes: 24FPS
void WindowManyCubes::DisplayByCppLoop(double current_time, int cubeCount) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);

    // fetch the reference to uniform variables
    const int mvLoc = glGetUniformLocation(rendering_program_, "mv_matrix");
    const int projLoc = glGetUniformLocation(rendering_program_, "proj_matrix");

    // create perspective matrix
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    const auto pMat = glm::perspective(FOVY, aspect, 0.1f, 1000.0f);

    for (int i = 0; i < cubeCount; ++i) {
        const auto tf = static_cast<float>(current_time + i);

        // create mv matrix
        const float distance = 10.0f;
        const auto vMat = translate(glm::mat4(1.0f), - camara_);
        const auto tMat = translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * tf) * distance, cos(0.52f * tf) * distance, sin(0.7f * tf) * distance));
        auto rMat = rotate(glm::mat4(1.0f), 1.75f * tf, glm::vec3(0.0f, 1.0f, 0.0f));
        rMat = rotate(rMat, 1.75f * tf, glm::vec3(1.0f, 0.0f, 0.0f));
        rMat = rotate(rMat, 1.75f * tf, glm::vec3(0.0f, 0.0f, 1.0f));
        auto sMat = scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
        const auto mMat = sMat * tMat * rMat;
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
}

// 0.1M cubes: 90FPS
// 1M cubes: 23FPS
void WindowManyCubes::DisplayByInstancing(double current_time, int cubeCount) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);

    // fetch the reference to uniform variables
    const int vLoc = glGetUniformLocation(rendering_program_, "v_matrix");
    const int projLoc = glGetUniformLocation(rendering_program_, "proj_matrix");
    const int tfLoc = glGetUniformLocation(rendering_program_, "tf");

    // create perspective matrix
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    const auto pMat = glm::perspective(FOVY, aspect, 0.1f, 1000.0f);

    // create mv matrix
    const auto vMat = translate(glm::mat4(1.0f), - camara_);

    // copy perspective matrix and mv matrix
    glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniform1f(tfLoc, static_cast<float>(current_time));

    // bind vbo to vertex attribute in shaders
    constexpr int location = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(location);

    // setup opengl configure
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, cubeCount);
}

