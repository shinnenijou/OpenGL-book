//
// Created by YuxinLiu on 2025/5/20.
//

#ifndef WINDOWONETRIANGLE_H
#define WINDOWONETRIANGLE_H

#include "WindowBase.h"

class WindowOneTriangle : public WindowBase {
    constexpr static int NUM_VAOS = 1;
public:
    WindowOneTriangle(int width, int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    GLuint rendering_program_ {};
    GLuint vao_[NUM_VAOS] {};
    float x_ {};
    float inc_ {0.01f};
};



#endif //WINDOWONETRIANGLE_H
