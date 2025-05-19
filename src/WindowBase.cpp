#include "WindowBase.h"

WindowBase::WindowBase(const int width, const int height, std::string title):width_(width),height_(height),title_(std::move(title)) {
    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);

    // The actual screen size created may be different from given size
    // so we need to get actual size after create the window
    glfwGetFramebufferSize(window_, &real_width_, &real_height_);

    // set actual buffer size to ensure consist
    glfwMakeContextCurrent(window_);
    glViewport(0, 0, real_width_, real_height_);
}

WindowBase::~WindowBase() {
    if (!window_) {
        return;
    }

    glfwDestroyWindow(window_);
    window_ = nullptr;
}

void WindowBase::Init() {

}

void WindowBase::Display(double current_time) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void WindowBase::Update() {
    Display(glfwGetTime());
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool WindowBase::IsClosed() const {
    return glfwWindowShouldClose(window_);
}

