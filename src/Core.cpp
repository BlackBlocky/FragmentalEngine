#include <frag/Core.h>

#include <iostream>

#include <GLFW/glfw3.h>

void Core::echo() {
    std::cout << "Echo!!\n"; 
}

void Core::window() {
    if (!glfwInit()) return;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // KEIN OpenGL

    GLFWwindow* window = glfwCreateWindow(800, 600, "Mein Fenster", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // hier Software-Rendering / Vulkan etc.
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}