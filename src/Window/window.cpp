#include "Window.hpp"

GLFWwindow* CORE::Window::window;

int CORE::Window::init(int width, int height, const char* title) {
    //GLFW init
    if (!glfwInit())
        return -1;

    // Create window
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window)
        glfwTerminate();

    glfwMakeContextCurrent(window);

    // GLAD init
    if (!gladLoadGL())
        return -1;
    return 1;
}

GLFWwindow* CORE::Window::GetWindow() {
    return glfwGetCurrentContext();
}

int CORE::Window::final() {
    glfwTerminate();
    return 0;
}

bool CORE::Window::isShouldClose() {
    return glfwWindowShouldClose(window);
}

void CORE::Window::pollEvents() {
    glfwPollEvents();
}

void CORE::Window::setShouldClose(bool flag) {
    glfwSetWindowShouldClose(window, flag);
}

void CORE::Window::swapBuffers() {
    glfwSwapBuffers(window);
}