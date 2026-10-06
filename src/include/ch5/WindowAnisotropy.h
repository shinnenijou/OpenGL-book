#ifndef WINDOWANISOTROPY_H
#define WINDOWANISOTROPY_H

#include "WindowBase.h"

class WindowAnisotropy : public WindowBase {
    constexpr static size_t VAO_NUM = 1;
    constexpr static size_t VBO_NUM = 2;
    constexpr static size_t VBO_INDEX_VERTEX = 0;
    constexpr static size_t VBO_INDEX_TEXTURE = 1;
    constexpr static float FOVY = 1.0472f; // 1.0472 radians = 60 degrees

public:
    WindowAnisotropy(const int width, const int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;

private:
    void SetupVertexBuffer(const float points[], int size, int index);

private:
    GLuint vao_[VAO_NUM];
    GLuint vbo_[VBO_NUM];
    GLuint rendering_program_;
    GLuint brickTexture_;
    glm::vec3 camara_ {};
    glm::vec3 init_loc_ {};
    glm::vec3 init_scale_ {};
};



#endif //WINDOWANISOTROPY_H
