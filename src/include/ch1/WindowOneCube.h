#ifndef WINDOWONECUBE_H
#define WINDOWONECUBE_H

#include "WindowBase.h"

class WindowOneCube : public WindowBase {
    constexpr static size_t VAO_NUM = 1;
    constexpr static size_t VBO_NUM = 2;
    constexpr static float FOVY = 1.0472f; // 1.0472 radians = 60 degrees
public:
    WindowOneCube(const int width, const int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    void SetupVertices(const float points[], int size, int index);
private:
    GLuint vao_[VAO_NUM];
    GLuint vbo_[VBO_NUM];
    GLuint rendering_program_;
    glm::vec3 camara_ {};
    glm::vec3 cube_loc_ {};
};



#endif //WINDOWONECUBE_H
