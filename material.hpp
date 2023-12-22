#ifndef _MATERIAL_HPP
#define _MATERIAL_HPP

#include <glm/vec3.hpp>
#include <string>
#include <iostream>

using glm::vec3;
using std::string;
using std::ostream;

struct Material {
    /** A material used for the Phong lighting model.
     */

    /** Constructor.
     */
    Material();

    /** The ambient reflectance */
    vec3 _ambient_reflectance;
    /** The diffuse reflectance */
    vec3 _diffuse_reflectance;
    /** The specular reflectance */
    vec3 _specular_reflectance;
    /** The shininess exponent */
    float _shininess;
    /** The name (for debugging) */
    string _name;

    /** Output to stream (for debugging).
     * @param os The stream
     * @param mat A Material
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Material& mat);
};

#endif
