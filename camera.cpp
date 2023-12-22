#include "camera.hpp"
#include <glm/geometric.hpp>  // cross, normalize
#include <glm/gtc/matrix_transform.hpp> // glm::translate, rotate
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp> // glm::to_string

using glm::vec4;
using glm::mat4;

using glm::normalize;
using glm::cross;
using glm::rotate;
using glm::radians;
using glm::length;
using glm::to_string;

using std::endl;

Camera::Camera() {
    _eye = vec3(0, 0, 5);
    _lookat = vec3(0, 0, 0);
    _up = vec3(0, 1, 0);
    _clip_Left = -1;
    _clip_Right = +1;
    _clip_Bottom = -1;
    _clip_Top = +1;
    _clip_Near = 4;

    init_basis();
}

void Camera::init_basis() {
    _basis_Z = normalize(_eye - _lookat);
    _basis_X = normalize(cross(_up, _basis_Z));
    _basis_Y = cross(_basis_Z, _basis_X);

    _vert_step = radians<float>(5);
    _horiz_step = _vert_step;
}

void Camera::orbit(const vec3& axis, float angle_step) {
    // Move eye point along a circle, centered at the look-at point.
    vec4 ref_to_eye = vec4(_eye - _lookat, 0.0);
    mat4 turn = rotate(mat4(1.0f), angle_step, axis);
    ref_to_eye = turn * ref_to_eye;
    _eye = vec3(vec4(_lookat, 0) + ref_to_eye);
    _basis_X = vec3(turn * vec4(_basis_X, 0));
    _basis_Y = vec3(turn * vec4(_basis_Y, 0));
    _basis_Z = vec3(turn * vec4(_basis_Z, 0));
}

void Camera::advance(float distance) {
    // Move the camera towards the look-at point.
    _eye += distance * normalize(_lookat - _eye);
}

void Camera::orbit_up() {
    // Orbit up, turning on camera's X axis
    orbit(_basis_X, -_vert_step);
}
void Camera::orbit_down() {
    // Orbit down, turning on camera's X axis
    orbit(_basis_X, _vert_step);
}

void Camera::orbit_right() {
    // Orbit right, turning on world up direction
    orbit(_up, _horiz_step);
}

void Camera::orbit_left() {
    // Orbit left, turning on world up direction
    orbit(_up, -_horiz_step);
}

void Camera::forward() {
    advance(length(_lookat - _eye) / 10);
}

void Camera::back() {
    advance(-length(_lookat - _eye) / 10);
}

ostream& operator<<(ostream& os, const Camera& orbiter) {
    os << "Camera(" << endl;
    os << "   eye:     " << to_string(orbiter._eye) << endl;
    os << "   lookat:  " << to_string(orbiter._lookat) << endl;
    os << "   vup:     " << to_string(orbiter._up) << endl;
    os << "   basis X: " << to_string(orbiter._basis_X) << endl;
    os << "   basis Y: " << to_string(orbiter._basis_Y) << endl;
    os << "   basis Z: " << to_string(orbiter._basis_Z) << endl;
    os << ")" << endl;
    return os;
}

