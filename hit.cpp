#include "hit.hpp"

Hit::Hit()
    : _t(-1)
{
    ;
}

void Hit::set(const vec3& position, const Material *material,
              const vec3& normal, float t) {
    _position = position;
    _material = material;
    _normal = normal;
    _t = t;
}
