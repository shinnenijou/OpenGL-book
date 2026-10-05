#ifndef WINDOWDUBEANDPYRAMID_H
#define WINDOWDUBEANDPYRAMID_H

#include "WindowBase.h"

class WindowCubeAndPyramid : public WindowBase {
    constexpr static size_t VAO_NUM = 1;
    constexpr static size_t VBO_NUM = 2;
    constexpr static size_t VBO_INDEX_CUBE = 0;
    constexpr static size_t VBO_INDEX_PYRAMID = 1;
    constexpr static float FOVY = 1.0472f; // 1.0472 radians = 60 degrees
public:
    WindowCubeAndPyramid(const int width, const int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    void SetupCubeVertices();
    void SetupPyramidVertices();

private:
    GLuint vao_[VAO_NUM];
    GLuint vbo_[VBO_NUM];
    GLuint rendering_program_;
    glm::vec3 camara_ {};
    glm::vec3 cube_loc_ {};
    glm::vec3 pyramid_loc_ {};
};



#endif //WINDOWDUBEANDPYRAMID_H
