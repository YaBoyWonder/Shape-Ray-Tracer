#include "cylinder.hpp"
#include "log.hpp"
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

using glm::dot;
using glm::length;
using glm::to_string;
using std::vector;
using std::endl;

#define EPSILON 0.001

Cylinder::Cylinder(const vec3& center, float radius,
                   float height,
                   const Material& material,
                   const string& name)
    : Shape(material, name), _center(center), _radius(radius), _height(height)
{
    ; // all done
}


bool Cylinder::intersects(const vec3 &start, const vec3 &direction, Hit &hit) {
    if (Log::LEVEL > 0) {
        Log::os() << "Entering Cylinder::intersects. start=" << to_string(start)
                  << " direction=" << to_string(direction) << endl;
    }

    vec3 center = _center;
    float radius = _radius;
    float rad2 = pow(radius, 2);

    float cylinder_divide = _height / 2;
    float a = direction.x * direction.x + direction.z * direction.z;
    float b = 2.0f * (direction.x * (start.x - center.x) + direction.z * (start.z - center.z));
    float c = (start.x - center.x) * (start.x - center.x) + (start.z - center.z) * (start.z - center.z) - rad2;
    float d = b*b - 4*a*c;
    float t1 = (-b + sqrt(d)) / (2.0f * a);
    float t2 = (-b - sqrt(d)) / (2.0f * a);
    float t = (t1 < t2) ? t1 : t2;

   // vec3 Q_top = (center + glm::vec3(EPSILON, cylinder_divide, EPSILON));
    vec3 Q_top = (center.y + vec3(EPSILON, cylinder_divide, EPSILON));
    vec3 Q_bottom = (center - glm::vec3(0, cylinder_divide, 0));

    float y = start.y + t * direction.y; // calcuate the y-value for constraint-checking
    //outside of the circle (the surrounding part of it between the top and bottom)
    if (y >= center.y - cylinder_divide && y <= center.y + cylinder_divide) {
        vec3 P_s_cylinder = start + t * direction;
        vec3 N = normalize(vec3(P_s_cylinder.x, 0, P_s_cylinder.z));
        hit.set(P_s_cylinder, &_material, N, t);
        return true;
    }


    float t_top = (center.y + cylinder_divide - start.y) / direction.y;
    float x_calc = start.x + t_top * direction.x;
    float z_calc = start.z + t_top * direction.z;
    bool inside_circle = (x_calc * x_calc + z_calc * z_calc < rad2);

    if (inside_circle) {
        vec3 P_s_cylinder = start + t_top * direction;
        hit.set(P_s_cylinder, &_material, normalize(P_s_cylinder - Q_top), t_top);
        return true;
    }  

    
    float t_bottom = (center.y - cylinder_divide - start.y) / direction.y;
    x_calc = start.x + t_bottom * direction.x, z_calc = start.z + t_bottom * direction.z;
    bool inside_circle_bottom  = ((x_calc * x_calc + z_calc * z_calc < rad2)); //inside the bottom part of the cylinder (bottom ring)0
    if (inside_circle_bottom) {
        vec3 P_s_cylinder = start + t_bottom * direction;
        hit.set(P_s_cylinder, &_material, normalize(Q_bottom), t_bottom);
        return true;
    }
    return false;
}


ostream& operator<<(ostream& os, const Cylinder& c) {
    os << "Cylinder(\"" << c._name << "\"\n"
       << "         center=" << to_string(c._center) << "\n"
       << "         radius=" << c._radius <<  "\n"
       << "         height=" << c._height << "\n"
       << "         material=" << c._material << ")";
    return os;
}
