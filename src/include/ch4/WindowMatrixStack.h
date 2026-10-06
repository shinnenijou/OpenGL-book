#ifndef WINDOWMATRIXSTACK_H
#define WINDOWMATRIXSTACK_H

#include "WindowBase.h"

class WindowMatrixStack : public WindowBase {
    constexpr static size_t VAO_NUM = 1;
    constexpr static size_t VBO_NUM = 3;
    constexpr static size_t VBO_INDEX_SUN = 0;
    constexpr static size_t VBO_INDEX_EARTH = 1;
    constexpr static size_t VBO_INDEX_MOON = 1;    // 和地球共用同一组顶点
    constexpr static size_t VBO_INDEX_JUPITER = 2;
    constexpr static float FOVY = 1.0472f; // 1.0472 radians = 60 degrees
public:
    WindowMatrixStack(const int width, const int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    void SetupVertices(const float points[], int size, int index);

private:
    GLuint vao_[VAO_NUM];
    GLuint vbo_[VBO_NUM];
    GLuint rendering_program_;
    glm::vec3 camara_ {};
    glm::vec3 sun_loc_ {};
};



#endif //WINDOWMATRIXSTACK_H
