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

// 正四面体
extern const float VERTICES_TETRAHEDRON_DEF[36];
extern const float TEXCOORDS_TETRAHEDRON_DEF[24];

// 立方体
extern const float VERTICES_CUBE_DEF[108];
extern const float TEXCOORDS_CUBE_DEF[72];

// 四棱锥
extern const float VERTICES_PYRAMID_DEF[54];
extern const float TEXCOORDS_PYRAMID_DEF[36];


#endif //WINDOW_H
