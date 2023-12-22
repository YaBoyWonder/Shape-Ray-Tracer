#ifndef _LIGHT_HPP
#define _LIGHT_HPP

#include <glm/vec3.hpp>
#include <string>

using glm::vec3;
using std::string;

struct Light {
    /** A point light.  */

    /** Constructor.
     */
    Light();

    /** Position */
    vec3 _position;
    /** Color */
    vec3 _color;
    /** The name (for debugging) */
    string _name;
};




#endif
