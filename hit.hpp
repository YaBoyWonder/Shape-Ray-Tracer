#ifndef _HIT_HPP
#define _HIT_HPP

#include "shape.hpp"
#include "material.hpp"

using glm::vec3;

class Hit {
    /** Stores information about a point where a ray intersects a shape
     */
 public:
    /** Default constructor.
     */
    Hit();

    /** Store information.
     * @param position Intersection point.
     * @param material Material of the intersection.
     * @param normal Surface normal at the intersection.
     * @param t Ray distance at the intersection.
     */
    void set(const vec3& position, const Material *material,
             const vec3& normal, float t);

    /** Intersection point */
    vec3 _position;
    /** Surface normal */
    vec3 _normal;
    /** Material of the intersection */
    const Material *_material;
    /** Ray distance */
    float _t;
};

#endif
