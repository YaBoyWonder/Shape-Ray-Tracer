#include "caster_controller.hpp"
#include "log.hpp"
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp> // glm::to_string
#include <iostream>

using glm::vec3;
using glm::to_string;
using std::cout;
using std::endl;

namespace Caster_Controller {

    // State that the controller uses:

    // A GLFW window where we'll draw things
    GLFWwindow *_GLFW_window;
    // Casts rays to make an image.
    Caster *_renderer;
    // Draws the ray-cast image onto the screen
    Caster_View *_view;
    // The image produced by the renderer
    SP_Image _image;

    // true/false if the image has been re-rendered,
    // and needs to be painted onto the screen again.
    bool _scene_changed;

    // Current mouse position
    int _mouse_x, _mouse_y;

    // Available image widths
    vector<int> _image_widths{20, 50, 100, 200, 300, 500, 800};
    int _current_image_width_index = 3;
    int _current_image_width;

    // Change the image resolution
    void update_resolution(int resolution_step) {
        // Step up or down in the list of widths,
        // but don't go off the end.
        _current_image_width_index
            = max(min(_current_image_width_index + resolution_step,
                      (int)_image_widths.size() - 1),
                  0);
        _current_image_width = _image_widths[_current_image_width_index];
        _renderer->update_image_dimensions(_current_image_width,
                                           _current_image_width);
    }

    // The camera moved: cast a new image.
    void rerender() {
        _renderer->camera_did_move();
        _image = _renderer->render();
        //_image->write_pnm("scene.ppm");
    }

    // Handle keyboard events.
    void key_callback(GLFWwindow* window, int key, int scancode,
                      int action, int mods)
    {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) {
                glfwSetWindowShouldClose(window, true);
            }
            else if (key == GLFW_KEY_LEFT) {
                _renderer->_camera.orbit_left();
            }
            else if (key == GLFW_KEY_RIGHT) {
                _renderer->_camera.orbit_right();
            }
            else if (key == GLFW_KEY_DOWN) {
                if (mods & GLFW_MOD_SHIFT)
                    _renderer->_camera.back();
                else
                    _renderer->_camera.orbit_down();
            }
            else if (key == GLFW_KEY_UP) {
                if (mods & GLFW_MOD_SHIFT)
                    _renderer->_camera.forward();
                else
                    _renderer->_camera.orbit_up();
            }
            else if (key == GLFW_KEY_R) {
                if (mods & GLFW_MOD_SHIFT)
                    update_resolution(+1);
                else
                    update_resolution(-1);
            }
            else if (key == GLFW_KEY_C) {
                cout << "Camera: "
                     << " eye=" << to_string(_renderer->_camera._eye)
                     << " lookat=" << to_string(_renderer->_camera._lookat)
                     << " up=" << to_string(_renderer->_camera._up) << endl;
                return;
            }
            else if (key == GLFW_KEY_I) {
                _image->write_pnm("scene.ppm");
            }
            else if (key == GLFW_KEY_S) {
                _renderer->toggle_shadowing();
            }

            rerender();
            _scene_changed = true;
        }
    }

    void framebuffer_size_callback(GLFWwindow* window,
                                   int width, int height) {
        // cout << "Window was resized to ("
        // << width << " x " << height << ")" << endl;

        _scene_changed = true;
    }

    void mouse_button_callback(GLFWwindow* window, int button,
                               int action, int mods ) {
        if (action == GLFW_PRESS) {
            // Convert the mouse (x y) to DCS coords.
            int window_width, window_height;
            glfwGetWindowSize(window, &window_width, &window_height);
            int x_DCS = _mouse_x * _current_image_width / window_width;
            int y_DCS = (window_height - _mouse_y - 1) * _current_image_width
                / window_height;

            cout << "----------------------------" << endl;
            cout << "Mouse clicked at ("
                 << _mouse_x << " " << _mouse_y << ")" << endl;
            cout << "DCS point     at ("
                 << x_DCS << " " << y_DCS << ")" << endl;

            // Cast one ray, with debugging on
            Log::LEVEL = 1;
            vec3 color = _renderer->ray_color(x_DCS, y_DCS);

            // Report the resulting color
            cout << "Pixel (" << x_DCS << " " << y_DCS << ") has color "
                 << glm::to_string(color) << endl;

            // And then disable debugging
            Log::LEVEL = 0;
        }
        ; // we'll ignore mouse events
    }

    void mouse_motion_callback(GLFWwindow* window,
                               double xpos, double ypos) {
        // Mouse moved.  Save its position.

        _mouse_x = xpos;
        _mouse_y = ypos;
    }

    void init(Caster& caster,
              Caster_View& view,
              GLFWwindow *window) {
        _renderer    = &caster;
        _view        = &view;
        _GLFW_window = window;

        // Set the image's resolution
        _current_image_width_index = 3;
        update_resolution(0);

        // Render the initial image
        _image = _renderer->render();
        _scene_changed = true;

        rerender();

        glfwSetFramebufferSizeCallback(_GLFW_window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);

        glfwSetMouseButtonCallback(_GLFW_window, mouse_button_callback);
        glfwSetCursorPosCallback(_GLFW_window, mouse_motion_callback);
    }

    void event_loop() {
        while (!glfwWindowShouldClose(_GLFW_window))
        {
            if (_scene_changed) {

                // cout << "Scene has changed.  Redraw" << endl;

                _view->draw(_image);
                glfwSwapBuffers(_GLFW_window);
            }

            // cout << "Gonna wait for a mouse or window event." << endl;

            _scene_changed = false;

            glfwWaitEvents();

        }

        // cout << "Event loop done. Window should be closed" << endl;

        return;
    }
};  // end namespace Caster_Controller

