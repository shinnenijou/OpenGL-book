#ifndef WINDOW_H
#define WINDOW_H

#include "opengl.h"
#include "macros.h"

#include <memory>
#include <functional>
#include <string>


/**
 * @brief Chapter 2 - program 1
 */
class WindowBase {
public:
    WindowBase(int width, int height, std::string title);
    DISALLOW_COPY(WindowBase);
    DISALLOW_MOVE(WindowBase);
    virtual ~WindowBase();

    virtual void Init();
    virtual void Display(double current_time);
    virtual void Update();

    bool IsClosed() const;

protected:
    double last_time_ {};
    int frame_ {};
    int width_ {};
    int height_ {};
    int real_width_ {};
    int real_height_ {};
    std::string title_ {};
    GLFWwindow * window_ {};
private:
    void Display();
};



#endif //WINDOW_H
