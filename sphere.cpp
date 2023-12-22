#include "sphere.hpp"
#include "log.hpp"
#include <math.h>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

using std::cout;
using std::endl;
using glm::to_string;

#define EPSILON 0.001

Sphere::Sphere(const vec3& center, float radius,
               const Material& material,
               const string& name)
    : Shape(material, name), _center(center), _radius(radius)
{
    ; // nothing left to do
}


bool Sphere::intersects(const vec3& start, const vec3& direction, Hit& hit) {
    if (Log::LEVEL > 0) {
        Log::os() << "sphere::intersect. ray.P: " << to_string(start)
                  << " ray.V:" << to_string(direction) << endl;
    }

    Material* sphere_material = new Material();
    
    float a = dot(direction, direction);
    float b = 2.0 * dot(direction, start - _center);
    float c = dot(start - _center, start - _center) - _radius * _radius;
    float d = b * b - 4 * a * c;
    if (d < 0) { return false; }; 
    float t1 = (-b - sqrt(d)) / (2 * a);
    float t2 = (-b + sqrt(d)) / (2 * a);
    float t = (t1 >= EPSILON) ? t1 : (t2 >= EPSILON) ? t2 : -1.0; 
    if (t < 0) { return false; }

    vec3 P_s = start + t * direction;
    vec3 N = normalize(P_s - _center);
    hit.set(P_s, &_material, N, t);
    return true;
}


ostream& operator<<(ostream& os, const Sphere& s) {
    os << "Sphere(\"" << s._name << "\"\n"
       << "       center=" << to_string(s._center) << "\n"
       << "       radius=" << s._radius << "\n"
       << "       material=" << s._material << ")";
    return os;
}
