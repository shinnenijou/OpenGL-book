//
// Created by YuxinLiu on 2025/5/19.
//

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include "opengl.h"
#include "debug.h"

#include <cstdlib>
#include <iostream>

template <typename Window>
int run(){
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    {
        Window window(300, 300, "Chapter-2");

        // WTF?
        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) {
            return EXIT_FAILURE;
        }
        glfwSwapInterval(1);

        window.Init();

        while (!window.IsClosed()) {
            window.Update();
        }
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}


#endif //RUNNER_HPP
