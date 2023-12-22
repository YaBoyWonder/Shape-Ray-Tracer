#ifndef _CYLINDER_HPP
#define _CYLINDER_HPP

#include <glm/vec3.hpp>
#include "shape.hpp"
#include "hit.hpp"
#include <iostream>

using std::ostream;

class Cylinder : public virtual Shape {
    /** A cylinder lined up with the Y axis. */
 public:
    /** Constructor.
     * @param center Position of the cylinder's center.
     * @param radius Radius of the cylinder.
     * @param height The height of the cylinder.
     * @param material Cylinder's material.
     */
    Cylinder(const vec3& center, float radius,
             float height,
             const Material& material,
             const string& name);

    /** Check if a ray intersects the cylinder.
     * If it does, return true, and set the hit parameter.
     * If not, return false, and don't change the hit parameter.
     * @param start Ray's starting point.
     * @param direction Ray's direction vector.
     * @param hit A Hit object. Call its .set() method if there's an intersection
     * @return true if there is an intersection, false otherwise.
     */
    bool intersects(const vec3& start,
                    const vec3& direction, Hit& hit);

    /** Center of the cylinder */
    vec3 _center;
    /** Radius of the cylinder */
    float _radius;
    /** Height of the cylinder */
    float _height;

    /** Output to stream (for debugging).
     * @param os The stream
     * @param c A cylinder
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Cylinder& c);

};

#endif
