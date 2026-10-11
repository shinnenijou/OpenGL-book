#include "opengl.h"
#include "Utils.h"
#include "WindowOneModel.h"
#include "debug.h"
#include "models.h"

#include <cstddef>


void WindowOneModel::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-5.7.glsl", "res/shader/fragShader-5.7.glsl");

    camara_ = {0.0f, 0.0f, 8.0f};
    init_loc_ = {0.0f, 0.0f, 0.0f};

    // Create and bind vao
    glGenVertexArrays(VAO_NUM, vao_);
    glBindVertexArray(vao_[0]);

    // Create VBOs
    glGenBuffers(VBO_NUM, vbo_);

    // Create a sphere model
    // 实际上是可以把顶点数组数据整个传入同一个VBO, 在为着色器启用顶点属性时指定不同偏移, 着色器就会将不同字段的数据解释为不同的顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.getVertexNum() * sizeof(Sphere::Vertex), model_.getVertices().data(), GL_STATIC_DRAW);

    // load texture
    textureID_ = Utils::loadTexture("res/texture/duck.jpg");
}

void WindowOneModel::Display(const double current_time)
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
    auto mvMat = translate(glm::mat4(1.0f), - camara_);
    mvMat *= glm::rotate(glm::mat4(1.0f), Utils::toRadian(90), {1, 0, 0});
    mvMat *= glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time) / 5, {0, 0, 1});
    mvMat *= glm::scale(glm::mat4(1.0f), {8, 8, 8});

    // copy perspective matrix and mv matrix
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // bind vbo to vertex attribute in shaders
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    int posLoc = 0;
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Sphere::Vertex), reinterpret_cast<void*>(offsetof(Sphere::Vertex, position)));
    glEnableVertexAttribArray(posLoc);
    int texLoc = 1;
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Sphere::Vertex), reinterpret_cast<void*>(offsetof(Sphere::Vertex, texCoord)));
    glEnableVertexAttribArray(texLoc);

    // bind texture data in memory to a texture unit (No. 0)
    // NOTE: 4.1 GLSL do not support layout (binding=0) syntax. do the same thing in application
    const int sampLoc = glGetUniformLocation(rendering_program_, "samp");
    glUniform1i(sampLoc, 0);    //  this zero is equivalent for layout(binding=0), meaning "Texture Unit 0"
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    // setup opengl configure
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, model_.getVertexNum());
}
