#include "opengl.h"
#include "Utils.h"
#include "WindowLightOnModel.h"
#include "debug.h"
#include "models.h"

#include <cstddef>

using namespace std;

void WindowLightOnModel::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-7.5.2.glsl", "res/shader/fragShader-7.5.2.glsl");

    camara_ = {0.0f, 0.0f, 8.0f};
    init_loc_ = {0.0f, 0.0f, 0.0f};
    globalAmbient_ = {
        {0.7f, 0.7f, 0.7f, 1.0f},
    };
    light_ = PosLight{
        {
            {0,0,0,1},
            {1,1,1,1},
            {1,1,1,1},
        },
        {5.0f, 2.0f, 2.0f},
        {}
    };

    light2_ = {
        {
            {0,0,0,1},
            {1,1,1,1},
            {1,1,1,1},
        },
        {-5.0f, -2.0f, 2.0f},
        {}
    };

    model_ = Torus(0.5, 0.2, 20);

    // Create and bind vao
    glGenVertexArrays(VAO_NUM, vao_);
    glBindVertexArray(vao_[0]);

    // Create VBOs
    glGenBuffers(VBO_NUM, vbo_);

    // Create a sphere model
    // 实际上是可以把顶点数组数据整个传入同一个VBO, 在为着色器启用顶点属性时指定不同偏移, 着色器就会将不同字段的数据解释为不同的顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, model_.getRawVertices().size() * sizeof(Sphere::Vertex), model_.getRawVertices().data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
    glBufferData(GL_ARRAY_BUFFER, model_.getRawIndices().size() * sizeof(Sphere::Vertex), model_.getRawIndices().data(), GL_STATIC_DRAW);

    double mousePosX = 0;
    double mousePosY = 0;
    glfwGetCursorPos(window_, &mousePosX, &mousePosY);
    mouse_pos_ = {mousePosX, mousePosY, 0};
}

void WindowLightOnModel::Display(const double current_time)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(rendering_program_);

    // create perspective matrix
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    auto pMat = glm::perspective(FOVY, aspect, 0.1f, 1000.0f);

    // create mv matrix
    auto vMat = translate(glm::mat4(1.0f), - camara_);
    auto mMat = glm::rotate(glm::mat4(1.0f), Utils::toRadian(90), {0, 1, 0});
    mMat *= glm::rotate(glm::mat4(1.0f), static_cast<float>(current_time) / 5, {0, 0, 1});
    mMat *= glm::scale(glm::mat4(1.0f), {4, 4, 4});

    // 计算当前帧鼠标位置变化量
    double mousePosX = 0;
    double mousePosY = 0;
    // glfwGetCursorPos(window_, &mousePosX, &mousePosY);
    glm::vec3 lightPosOffset = glm::vec3{mousePosX, mousePosY, 0};

    // init lighting based on current light position
    InstallLight(vMat, lightPosOffset);

    // create inverse transpose of mv matrix to transform normal vector
    auto mvMat = vMat * mMat;
    auto invTrMat = glm::transpose(glm::inverse(mvMat));

    // copy perspective matrix and mv matrix
    const int mvLoc = glGetUniformLocation(rendering_program_, "mv_matrix");
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

    const int projLoc = glGetUniformLocation(rendering_program_, "proj_matrix");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    const int nLoc = glGetUniformLocation(rendering_program_, "norm_matrix");
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

    // bind vbo to vertex attribute in shaders
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
    int posLoc = 0;
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(decltype(model_)::Vertex), reinterpret_cast<void*>(offsetof(decltype(model_)::Vertex, position)));
    glEnableVertexAttribArray(posLoc);
    int normLoc = 1;
    glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(decltype(model_)::Vertex), reinterpret_cast<void*>(offsetof(decltype(model_)::Vertex, normal)));
    glEnableVertexAttribArray(normLoc);

    // setup opengl configure
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[1]);
    glDrawElements(GL_TRIANGLES, model_.getRawIndices().size(), GL_UNSIGNED_INT, 0);
}

void WindowLightOnModel::InstallLight(const glm::mat4 &vMat, const glm::vec3& offset) {
    // 将光源位置转换为视图空间坐标, 并存入浮点数组
    auto lightPos = glm::vec3(vMat * glm::vec4(light_.position, 1.0f)) + offset;
    auto light2Pos = glm::vec3(vMat * glm::vec4(light2_.position, 1.0f)) + offset;

    GLint loc {};

    // 全局环境光
    loc = glGetUniformLocation(rendering_program_, "globalAmbient");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(globalAmbient_.ambient));

    // 位置光的各个分量
    loc = glGetUniformLocation(rendering_program_, "light.ambient");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(light_.ambient));
    loc = glGetUniformLocation(rendering_program_, "light.diffuse");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(light_.diffuse));
    loc = glGetUniformLocation(rendering_program_, "light.specular");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(light_.specular));
    loc = glGetUniformLocation(rendering_program_, "light.position");
    glProgramUniform3fv(rendering_program_, loc, 1, glm::value_ptr(lightPos));

    // 位置光2的各个分量
    loc = glGetUniformLocation(rendering_program_, "light2.ambient");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(light2_.ambient));
    loc = glGetUniformLocation(rendering_program_, "light2.diffuse");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(light2_.diffuse));
    loc = glGetUniformLocation(rendering_program_, "light2.specular");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(light2_.specular));
    loc = glGetUniformLocation(rendering_program_, "light2.position");
    glProgramUniform3fv(rendering_program_, loc, 1, glm::value_ptr(light2Pos));

    // 材质的各个分量
    loc = glGetUniformLocation(rendering_program_, "material.ambient");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(mat_.ambient));
    loc = glGetUniformLocation(rendering_program_, "material.diffuse");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(mat_.diffuse));
    loc = glGetUniformLocation(rendering_program_, "material.specular");
    glProgramUniform4fv(rendering_program_, loc, 1, glm::value_ptr(mat_.specular));
    loc = glGetUniformLocation(rendering_program_, "material.shininess");
    glProgramUniform1f(rendering_program_, loc, mat_.shininess);
}

