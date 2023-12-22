#ifndef _TRIANGLE_HPP
#define _TRIANGLE_HPP

#include "hit.hpp"
#include "shape.hpp"

class Triangle : public virtual Shape {
  /** A triangle in 3D space. */
public:
  /** Constructor.
   * @param v1 First vertex.
   * @param v2 Second vertex.
   * @param v3 Third vertex.
   * @param material Triangle's material.
   */
  Triangle(const vec3 &v1, const vec3 &v2, const vec3 &v3,
           const Material &material, const string &name);

  /** Check if a ray intersects the triangle.
   * If it does, return true, and set the hit parameter.
   * If not, return false, and don't change the hit parameter.
   * @param start Ray's starting point.
   * @param direction Ray's direction vector.
   * @param hit A Hit object. Call its .set() method if there's an intersection
   * @return true if there is an intersection, false otherwise.
   */
  bool intersects(const vec3 &start, const vec3 &direction, Hit &hit);
  /** Check if a ray intersect the triangle.
   * Unlike the intersects(), this projects the triangle
   * onto 2D, and counts how many 2D edges cross a ray
   * that starts at the ray-plane hit point.
   * @param start Ray's starting point.
   * @param direction Ray's direction vector.
   * @param hit A Hit object. Call its .set() method if there's an intersection
   * @return true if there is an intersection, false otherwise.
   */
  bool intersects2(const vec3 &start, const vec3 &direction, Hit &hit);

  /** Vertices */
  vec3 _A, _B_2, _C_2;

  /** Output to stream (for debugging).
   * @param os The stream
   * @param t A Triangle
   * @return the stream, after output.
   */
  friend ostream &operator<<(ostream &os, const Triangle &t);

private:
  vec3 _N_2, _Q;
};

#endif
