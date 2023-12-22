#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 *  A wrapper around a GLFWwindow object.
 */

class Window {
public:
    /** Constructor.  Create the window, specifying various attributes.
     * @param major_version Major OpenGL version number (eg for 3.1 this is 3)
     * @param minor_version Minor OpenGL version number (eg for 3.1 this is 1)
     * @param width Width in pixels for the window
     * @param height Height in pixels for the window
     * @param title Label for the window's title bar
     */
    Window(int major_version,
           int minor_version,
           const unsigned int width,
           const unsigned int height,
           const GLchar *title);

    /** Retrieve the GLFWindow object.
     * @return Pointer to GLFW's window handle.
     */
    GLFWwindow *get_GLFW_window();

private:
    GLFWwindow *_window;
};

#endif
