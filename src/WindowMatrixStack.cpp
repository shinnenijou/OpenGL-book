#include "opengl.h"
#include "Utils.h"
#include "debug.h"
#include "WindowMatrixStack.h"

#include <stack>
#include <cmath>


void WindowMatrixStack::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-4.5.glsl", "res/shader/fragShader-4.5.glsl");

    camara_ = {0.0f, 0.0f, 16.0f};
    sun_loc_ = {0.0f, 0.0f, 0.0f};

    // Create and bind vao
    glGenVertexArrays(VAO_NUM, vao_);
    glBindVertexArray(vao_[0]);

    // Create vbo
    glGenBuffers(VBO_NUM, vbo_);

    SetupPyramidVertices(VBO_INDEX_SUN);
    SetupCubeVertices(VBO_INDEX_EARTH);
    SetupTetrahedron(VBO_INDEX_JUPITER);
}

void WindowMatrixStack::Display(const double current_time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);

    // fetch the reference to uniform variables
    const int mvLoc = glGetUniformLocation(rendering_program_, "mv_matrix");
    const int projLoc = glGetUniformLocation(rendering_program_, "proj_matrix");
    const int positionLoc = 0;
    const auto tf = static_cast<float>(current_time);

    // create perspective matrix
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    const auto pMat = glm::perspective(FOVY, aspect, 0.1f, 1000.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    std::stack<glm::mat4> mvStack{};

    // option 1. constant view
    // auto vMat = translate(glm::mat4(1.0f), - camara_);

    // option 2. loot at the sun
    // auto vMat = glm::lookAt(camara_, sun_loc_, glm::vec3(0,1,0));

    // option 3. look at the earch
    auto posEarth = glm::vec3(sin(tf) * 4, 0.0, cos(tf) *4);
    auto vMat = glm::lookAt(glm::vec3(0.0f, 16.0f, 0.0f), posEarth, glm::vec3(0,0,1));

    // 入栈视图矩阵
    mvStack.push(vMat);                                                   // 此时栈顶: 视图

    // =============== 四棱锥 / 太阳 ===============
    mvStack.push(mvStack.top());                                        // 此时栈顶:  太阳位置
    mvStack.top() *= glm::translate(glm::mat4(1.0f), sun_loc_);
    mvStack.push(mvStack.top());                                        // 此时栈顶:  太阳自转
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), tf, glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_SUN]);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLoc);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    mvStack.pop();                                                         // 此时栈顶: 太阳位置

    // =============== 大立方体 / 地球 ===============
    mvStack.push(mvStack.top());                                         // 此时栈顶: 地球位置(含公转)
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin(tf) * 4, 0.0, cos(tf) *4));
    mvStack.push(mvStack.top());                                         // 此时栈顶: 地球自转
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), tf, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_EARTH]);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLoc);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();                                                          // 此时栈顶: 地球位置(含公转)

    // =============== 小立方体 / 月球 ===============
    mvStack.push(mvStack.top());                                          // 此时栈顶: 月球
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin(tf) * 2, cos(tf) * 2));
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), tf, glm::vec3(0.0f, 0.0f, 1.0f));
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_MOON]);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLoc);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    mvStack.pop();                                                          // 此时栈顶: 地球位置(含公转)

    // =============== 正四边形 / 木星 ===============
    mvStack.pop();                                                          // 此时栈顶: 太阳位置
    mvStack.push(mvStack.top());                                          // 此时栈顶: 木星位置(含公转)
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin(tf * 0.2) * 12, 0.0, cos(tf * 0.2) * 12));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_JUPITER]);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(positionLoc);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void WindowMatrixStack::SetupCubeVertices(size_t index)
{
    CHECK_ARRAY_INDEX_NORET(vbo_, index);
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}

void WindowMatrixStack::SetupPyramidVertices(size_t index) {
    CHECK_ARRAY_INDEX_NORET(vbo_, index);
    constexpr float vertex_positions[] = {
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 前面
        1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 右面
        1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 后面
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 左面
        -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // 底面左前
        1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f // 底面右后
    };

    // bind vertex postions to the second vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}

void WindowMatrixStack::SetupTetrahedron(size_t index) {
    CHECK_ARRAY_INDEX_NORET(vbo_, index);

    constexpr float vertex_positions[] = {
        0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0,
    };

    // bind vertex postions to the second vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
}