#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>

#include "Window.h"

using namespace std;

void init (GLFWwindow * window){}

void display(GLFWwindow* window, double crrentTime) {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

using CurrentWindow = WindowBase;

int main() {
    if (!glfwInit()) { return EXIT_FAILURE; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    {
        CurrentWindow window(300, 300, "Chapter-2");

        // WTF?
        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) { return EXIT_FAILURE; }
        glfwSwapInterval(1);

        window.Init();

        while (!window.IsClosed()) {
            window.Update();
        }
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}