#ifndef WINDOWONETRIANGLE_H
#define WINDOWONETRIANGLE_H

#include "WindowBase.h"

class WindowOneTriangle : public WindowBase {
    constexpr static int NUM_VAOS = 1;
    constexpr static float SPEED = 1.0f;
public:
    WindowOneTriangle(int width, int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    GLuint rendering_program_ {};
    GLuint vao_[NUM_VAOS] {};
    float x_ {};
    float direction_ {1.0f}; // per second
    double last_time_ {};
};



#endif //WINDOWONETRIANGLE_H
