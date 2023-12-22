#include "material.hpp"

#include <glm/gtx/string_cast.hpp>
using glm::to_string;

Material::Material() {
    _ambient_reflectance = vec3(0.1, 0.1, 0.1);
    _diffuse_reflectance = vec3(0.5, 0.5, 0.5);
    _specular_reflectance = vec3(0.5, 0.5, 0.5);
    _shininess = 10;
    _name = "NO NAME";
}

ostream& operator<<(ostream& os, const Material& mat) {
    os << "Material(\"" << mat._name << "\"\n"
       << "         ka=" << to_string(mat._ambient_reflectance) << "\n"
       << "         kd=" << to_string(mat._diffuse_reflectance) << "\n"
       << "         ks=" << to_string(mat._specular_reflectance) << "\n"
       << "         n=" << mat._shininess << ")";
    return os;
}
