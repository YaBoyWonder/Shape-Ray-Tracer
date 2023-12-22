#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <glm/vec3.hpp>
#include <iostream>

using glm::vec3;
using std::ostream;

struct Camera {
    /** Constructor */
    Camera();

    /** The eye moved, re-compute the VCS basis vectors
     * needed for orbiting.
     */
    void init_basis();

    /** Move the eye towards the reference point.
     * @param distance Amount to move the eye.
     */
    void advance(float distance);

    /** Move the eye point up, pivoting around the world X axis.
     */
    void orbit_up();

    /** Move the eye point down, pivoting around the world X axis.
     */
    void orbit_down();

    /** Move the eye point right, pivoting around the up-vector.
     */
    void orbit_right();

    /** Move the eye point left, pivoting around the up-vector.
     */
    void orbit_left();

    /** Move the eye point forward (dolly in) along the viewing axis.
     */
    void forward();

    /** Move the eye point back (dolly out) along the viewing axis.
     */
    void back();

    /** Output function, for debugging.
     */
    friend ostream& operator<<(ostream& os, const Camera& camera);

    /** The eye position
     */
    vec3 _eye;

    /** The look-at point.
     */
    vec3 _lookat;

    /** Which way is "up" in the world.
     */
    vec3 _up;

    /** X coord of left side of image window.*/
    float _clip_Left;
    /** X coord of right side of image window.*/
    float _clip_Right;
    /** Y coord of bottom side of image window.*/
    float _clip_Bottom;
    /** Y coord of top side of image window.*/
    float _clip_Top;
    /** Distance from eye to image window.*/
    float _clip_Near;

    /** How much each light contributes to the over-all ambient light */
    float _ambient_fraction;
 private:
    /** Orbit the eye point around the reference point.
     * @param axis Axis to rotate around.
     * @param angle_step Amount to rotate (radians).
     */
    void orbit(const vec3& axis, float angle_step);

    vec3 _basis_X, _basis_Y, _basis_Z;
    float _vert_step, _horiz_step;

};

#endif
