#ifndef WINDOWMANYCUBES_H
#define WINDOWMANYCUBES_H

#include "WindowBase.h"

class WindowManyCubes : public WindowBase {
    constexpr static size_t VAO_NUM = 1;
    constexpr static size_t VBO_NUM = 2;
    constexpr static float FOVY = 1.0472f; // 1.0472 radians = 60 degrees
public:
    WindowManyCubes(const int width, const int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    void SetupVertices(const float points[], int size, int index);
    void DisplayByCppLoop(double current_time, int cubeCount);
    void DisplayByInstancing(double current_time, int cubeCount);

private:
    GLuint vao_[VAO_NUM];
    GLuint vbo_[VBO_NUM];
    GLuint rendering_program_;
    glm::vec3 camara_ {};
    glm::vec3 cube_loc_ {};
};



#endif //WINDOWMANYCUBES_H
