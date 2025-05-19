#ifndef WINDOWONEDOT_H
#define WINDOWONEDOT_H

#include "WindowBase.h"

class WindowOnePoint : public WindowBase {
    constexpr static int NUM_VAOS = 1;
public:
    WindowOnePoint(int width, int height, std::string title): WindowBase(width, height, std::move(title)) {}
    void Init() override;
    void Display(double current_time) override;
private:
    GLuint CreateShaderProgram();
private:
    GLuint rendering_program_ {};
    GLuint vao[NUM_VAOS] {};
};



#endif //WINDOWONEDOT_H
