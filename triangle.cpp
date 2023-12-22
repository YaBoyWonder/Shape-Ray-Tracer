#include "triangle.hpp"
#include "log.hpp"

#include <cmath>
#include <fstream>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <iostream>

using glm::cross;
using glm::determinant;
using glm::mat3;
using glm::to_string;
using std::cout;
using std::endl;

#define EPSILON 0.001

Triangle::Triangle(const vec3 &v1, const vec3 &v2, const vec3 &v3,
                   const Material &material, const string &name)
    : Shape(material, name), _A(v1), _B_2(v2), _C_2(v3) {
  vec3 ab = v2 - v1;
  vec3 ac = v3 - v1;
  _N_2 = normalize(cross(ab, ac));
  _Q = _A;
}


bool Triangle::intersects(const vec3 &start, const vec3 &direction, Hit &hit) {
    if (Log::LEVEL > 0) {
        Log::os() << "Entering Triangle::intersects. start=" << to_string(start)
                  << " direction=" << to_string(direction) << endl;
    }

    Material* triangleMaterial = new Material();

    vec3 A = _A;
    vec3 B = _B_2;
    vec3 C = _C_2;

    vec3 AB = B - A;
    vec3 AC = C - A;
    // vec3 surface_normal = glm::normalize(cross(AB, AC));

    glm::mat3 D = glm::mat3(1.0);
    D[0] = direction;
    D[1] = A - B;
    D[2] = A - C;

    glm::mat3 D_t = glm::mat3(1.0);
    D_t[0] = A - start;
    D_t[1] = A - B;
    D_t[2] = A - C;

    glm::mat3 D_u = glm::mat3(1.0);
    D_u[0] = direction;
    D_u[1] = A - start;
    D_u[2] = A - C;

    glm::mat3 D_v = glm::mat3(1.0);
    D_v[0] = direction;
    D_v[1] = A - B;
    D_v[2] = A - start;

    float u = determinant(D_u) / determinant(D);
    float v = determinant(D_v) / determinant(D);
    float t_cramer = determinant(D_t) / determinant(D);
    
    /*    if (Log::LEVEL > 0) {
      Log::os() << "C_1: " << to_string(cond_1) << "\nC_2: " << to_string(cond_2) << endl;
    } */

    if (t_cramer >= EPSILON) {
        if (u >= 0 && v >= 0 && u + v <= 1) {
            vec3 P_t = start + t_cramer * direction;
            // hit.set(P_t, *material, surface_normal, t_cramer);
            // Material triangleMaterial = _material;
//            const Material testing32 = _material; 
//            hit.set(P_t, &testing32, surface_normal, t_cramer);

              hit.set(P_t, &_material, _N_2, t_cramer);
              //hit.set(P_t, &_material, surface_normal, t_cramer);
            //hit.set(P_t, triangleMaterial, surface_normal, t_cramer);
            return true;
        }
    }
    return false;
}

ostream &operator<<(ostream &os, const Triangle &t) {
  os << "Triangle(\"" << t._name << "\"\n"
     << "         A=" << to_string(t._A) << "\n"
     << "         B=" << to_string(t._B_2) << "\n"
     << "         C=" << to_string(t._C_2) << "\n"
     << "         material=" << t._material << ")";
  return os;
}
