#include <Engine/Window/window.hpp>

Window::Window(u16 width, u16 height, const char* title) {
    glfwInit();
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

Window::~Window() {
    
}

void Window::present() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::attachGlContext() {
    glfwMakeContextCurrent(window);
}

bool Window::closed() {
    return (bool) glfwWindowShouldClose(window);
}