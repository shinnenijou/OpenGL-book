#include "opengl.h"
#include "Utils.h"
#include "debug.h"
#include "WindowAnisotropy.h"

void WindowAnisotropy::Init()
{
    WindowBase::Init();
    rendering_program_ = Utils::CreateShaderProgram("res/shader/vertShader-5.7.glsl", "res/shader/fragShader-5.7.glsl");

    float h = 10.0f;
    camara_ = {0.0f, -(h + 3), -(h)};
    init_loc_ = {0.0f, 0.0f, 0.0f};
    init_scale_ = {h, h, h};

    // Create and bind vao
    glGenVertexArrays(VAO_NUM, vao_);
    glBindVertexArray(vao_[0]);

    // Create
    glGenBuffers(VBO_NUM, vbo_);

    SetupVertexBuffer(VERTICES_PYRAMID_DEF, sizeof(VERTICES_PYRAMID_DEF), VBO_INDEX_VERTEX);
    SetupVertexBuffer(TEXCOORDS_PYRAMID_DEF, sizeof(TEXCOORDS_PYRAMID_DEF), VBO_INDEX_TEXTURE);

    // Load texture data from file system
    brickTexture_ = Utils::loadTexture("res/texture/133-pin.jpg");
}

void WindowAnisotropy::SetupVertexBuffer(const float points[], int size, int index) {
    CHECK_ARRAY_INDEX_NORET(vbo_, index);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[index]);
    glBufferData(GL_ARRAY_BUFFER, size, points, GL_STATIC_DRAW);
}

void WindowAnisotropy::Display(const double current_time)
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
    const auto vMat = glm::lookAt(camara_, {0.0f, camara_.y / 2, 0.0f}, glm::vec3(0, 0, 1));
    auto mMat = translate(glm::mat4(1.0f), init_loc_);
    mMat *= glm::scale(glm::mat4(1.0f), init_scale_);

    auto mvMat = vMat * mMat;

    // copy perspective matrix and mv matrix
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // bind vbo to vertex attribute in shaders
    int location = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_VERTEX]);
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(location);

    // bind the 2dn vbo to texture coordinates in shaders
    location = 1;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[VBO_INDEX_TEXTURE]);
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(location);

    // bind texture data in memory to a texture unit (No. 0)
    // NOTE: 4.1 GLSL do not support layout (binding=0) syntax. do the same thing in application
    const int sampLoc = glGetUniformLocation(rendering_program_, "samp");
    glUniform1i(sampLoc, 0);    //  this zero is equivalent for layout(binding=0), meaning "Texture Unit 0"
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture_);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_OF(VERTICES_PYRAMID_DEF));
}