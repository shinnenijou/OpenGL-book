#include "WindowBase.h"
#include "debug.h"

#include <iostream>

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
    const auto current_time = glfwGetTime();
    ++frame_;

    if (current_time - last_time_ > 5)
    {
        LOG_INFO("Current frame rate: %d", frame_ / 5);
        last_time_ = current_time;
        frame_ = 0;
    }

    Display(current_time);
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

bool WindowBase::IsClosed() const {
    return glfwWindowShouldClose(window_);
}

