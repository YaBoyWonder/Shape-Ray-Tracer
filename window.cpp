#include <iostream>
#include "window.hpp"

Window::Window(int major_version,
               int minor_version,
               const unsigned int width,
               const unsigned int height,
               const GLchar *title) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for MacOS
#endif

    // Get 24 bits of depth buffer.
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    // Window should be double-buffered
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);


    // glfw window creation
    // --------------------
    _window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(_window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }
}

GLFWwindow* Window::get_GLFW_window() {
    return _window;
}
