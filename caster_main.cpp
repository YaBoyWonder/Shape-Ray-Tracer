// Draws a scene using ray casting

#include <iostream>
#include <string>
#include "caster_view.hpp"
#include "caster_controller.hpp"
#include "caster.hpp"
#include "window.hpp"
#include "log.hpp"
#include <GLFW/glfw3.h>

using std::cin;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cerr << "Usage:" << endl;
        cerr << "   caster <scene_file.txt>" << endl;
        cerr << " PRESS Control-C to exit program:";
        string line;
        getline(cin, line);
        exit(1);
    }

    // Start up the glfw system.
    glfwInit();

    int display_width = 800;
    int display_height = 800;

    // Make a window for displaying the graphics
    Window window(3, 3,
                  display_width,
                  display_height,
                  "Ray Cast scene");

    // Initial image dimensions
    int image_width = 200;
    int image_height = 200;

    // Create a ray-casting renderer.
    Caster caster(image_width, image_height);
    caster.read_scene(argv[1]);

    // Create a view to paint the image onto the screen.
    Caster_View view;

    // And finally initialize the controller that orchestrates
    // the events and joins the pieces.
    Caster_Controller::init(caster, view, window.get_GLFW_window());

    // Go handle the events.  event_loop() will return
    // when the user closes the window.
    Caster_Controller::event_loop();

    // Close the window
    glfwDestroyWindow(window.get_GLFW_window());

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();


    return 0;
}
