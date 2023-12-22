#ifndef _CASTER_HPP
#define _CASTER_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "shape.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "light.hpp"

using glm::vec3;
using glm::mat4;
using std::string;

/** A ray caster.
 * Given a scene with several shapes,
 * and specifications for a camera,
 * uses ray casting to create an image.
 */

class Caster {
 public:
    /** Constructor.
     * @param width How many pixels in the image, across.
     * @param height How many pixels up-down.
     */
    Caster(int width, int height);

    /** Destructor.
     */
    ~Caster();

    /** Set a ray's start position and direction vector.
     * @param x_dcs DCS X coordinate (column) of the pixel.
     * @param y_dcs DCS Y coordinate (row) of the pixel.
     * @param S Ray's start point (one output of this function).
     * @param V Ray's direction vector (the other output of this function).
     */
    void set_ray(int x_dcs, int y_dcs,
                 vec3& S, vec3& V);

    /** This is called when the image should be re-sized.
     * @param width New width (number of pixel columns) of the image.
     * @param height New height (number of pixel rows) of the image.
     */
    void update_image_dimensions(int width, int height);

    /** This is called when the camera position changes.
     */
    void camera_did_move();

    /** Finds the first hit for the given ray.
     * @param start Ray's starting point.
     * @param direction Ray's direction vector.
     * @param hit Hit record, which will be set if there's a hit.
     * @return true/false if the ray does/doesn't hit some Shape.
     */
    bool get_first_hit(const vec3& start, const vec3& direction,
                       Hit& hit);

    /** Returns the color of a pixel.
     * @param x_dcs DCS X coordinate (column) of the pixel.
     * @param y_dcs DCS Y coordinate (row) of the pixel.
     * @return The pixels' RGB.
     */
    vec3 ray_color(int x_dcs, int y_dcs);

    /** Re-render the image.
     * @return The new image
     */
    SP_Image render();

    /** Compute the total color for one hit point, taking all lights
     * into account.
     * @param S ray start point.
     * @param V ray direction vector.
     * @param hit The hit information.
     */
    vec3 glossy_color(const vec3& S, const vec3& V, const Hit& hit);

    /** Get the reflected ray.
     * @param L unit vector towards light source.
     * @param N surface normal.
     * @return The reflection of L on N.
     */
    vec3 mirror_direction(const vec3& L, const vec3& N);

    /** Get one light's contribution to the color.
     * @param V unit vector towards eye point (negative of ray direction vector)
     * @param N surface normal at hit point.
     * @param L unit vector towards the light.
     * @param light_color color of the light.
     * @param mat Material at the hit point.
     */
    vec3 local_illumination(const vec3& V, const vec3& N,
                            const vec3& L, const vec3& light_color,
                            const Material& mat);


    vec3 calculate_ray_direction(int x_dcs, int y_dcs);

    /** Read a scene file, which describes lights, colors, the camera,
     * and shapes.
     * @param file_name Name of the scene file.
     */
    void read_scene(const string& file_name);

    /** Public access to the renderer's camera position.*/
    Camera _camera;

    /** Flip the shadow status */
    void toggle_shadowing();

 private:

    /** Allocate the pixels for the image.
     * @param width Number of columns in the image.
     * @param height Number of rows in the image.
     */
    void allocate_image(int width, int height);

    /** Destroy the image.
     */
    void deallocate_image();

    /** Does a ray hit SOME object? (used for shadows).
     * @param start Ray's start point.
     * @direction Ray's direction vector.
     * @return whether something was hit.
     */
    bool hits_something(const vec3& start, const vec3& direction);

    unsigned char *_pixels;
    int _width, _height;

    vector <Shape*> _scene;
    vector <Light> _lights;
    mat4 _M_vcs_to_wcs;
    vec3 _background_color;
    vector<Hit> _hit_list;
    float _pixel_width, _pixel_height;
    vec3 _ambient_light;
    bool _shadowing;
};

#endif
