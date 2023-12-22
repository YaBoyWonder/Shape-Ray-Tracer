#ifndef _SPHERE_HPP
#define _SPHERE_HPP

#include <glm/vec3.hpp>
#include "shape.hpp"
#include "hit.hpp"
#include <iostream>

using std::ostream;

class Sphere : public virtual Shape {
    /** A sphere */
 public:
    /** Constructor.
     * @param center Position of the sphere's center.
     * @param radius Radius of the sphere.
     * @param material Sphere's material.
     */
    Sphere(const vec3& center, float radius,
           const Material& material,
           const string& name);

    /** Check if a ray intersects the sphere.
     * If it does, return true, and set the hit parameter.
     * If not, return false, and don't change the hit parameter.
     * @param start Ray's starting point.
     * @param direction Ray's direction vector.
     * @param hit A Hit object. Call its .set() method if there's an intersection
     * @return true if there is an intersection, false otherwise.
     */
    bool intersects(const vec3& start,
                    const vec3& direction, Hit& hit);

    /** Sphere's center point */
    vec3 _center;
    /** Sphere's radius */
    float _radius;

    /** Output to stream (for debugging).
     * @param os The stream
     * @param s A Sphere
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Sphere& s);
};

#endif
