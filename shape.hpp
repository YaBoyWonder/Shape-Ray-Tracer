#ifndef _SHAPE_HPP
#define _SHAPE_HPP

#include "material.hpp"

class Hit;

#include <string>
using glm::vec3;
using std::string;

class Shape {
    /** A 3D shape.
     * This is an abstract class, because it doesn't implement
     * the intersects() method.
     */
 public:
    /** Constructor.
     * @param material The Material of the shape.
     * @param name The shape's name.
     */
    Shape(const Material& material,
          const string& name);

    /** Check if a ray intersects the shape.
     * THIS METHOD IS ABSTRACT, so child classes MUST implement it.
     * If it does, return true, and set the hit parameter.
     * If not, return false, and don't change the hit parameter.
     * @param start Ray's starting point.
     * @param direction Ray's direction vector.
     * @param hit A Hit object. Call its .set() method if there's an intersection
     * @return true if there is an intersection, false otherwise.
     */
    virtual bool intersects(const vec3& start,
                            const vec3& direction, Hit& hit) = 0;

    /** The shape's material */
    Material _material;
    /** The shape's name (for debugging) */
    string _name;
};

#endif
