#ifndef _CASTER_CONTROLLER_HPP
#define _CASTER_CONTROLLER_HPP

#include "camera.hpp"
#include "window.hpp"
#include "caster_view.hpp"
#include "caster.hpp"
#include "image.hpp"
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>

using glm::vec3;

namespace Caster_Controller {
    // Initialize.
    // Sets the state (the view and the window)
    void init(Caster& caster,
              Caster_View& view,
              GLFWwindow *window);

    // Get events from GLFW, and tell the view
    // what to do.  Return when user closes the window.
    void event_loop();
};

#endif
