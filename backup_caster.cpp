#include "caster.hpp"
#include "triangle.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "scene_reader.hpp"
#include "log.hpp"

#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp> // glm::to_string

using glm::vec4;
using glm::cross;
using glm::normalize;
using glm::to_string;
using std::cout;
using std::cerr;
using std::endl;

Caster::Caster(int width, int height) {
    _pixels = nullptr;
    _width = 0;
    _height = 0;
    update_image_dimensions(width, height);
    _background_color = vec3(0.7, 0.6, 0.4);
    _shadowing = true;
}

void Caster::allocate_image(int width, int height) {
    deallocate_image();
    int num_pixels = width * height * 3;
    _pixels = new unsigned char[num_pixels];
    _width = width;
    _height = height;
}

void Caster::deallocate_image() {
    if (_pixels != nullptr) {
        delete[] _pixels;
    }
}

Caster::~Caster() {
    deallocate_image();
}

void Caster::update_image_dimensions(int width, int height) {
    if (width != _width || height != _height) {
        allocate_image(width, height);
        _pixel_width  = (_camera._clip_Right - _camera._clip_Left) / width;
        _pixel_height = (_camera._clip_Top - _camera._clip_Bottom) / height;
    }
}

void Caster::toggle_shadowing() {
    _shadowing = !_shadowing;
}

// void Caster::set_ray(int x_dcs, int y_dcs,
//                      vec3& S, vec3& V) {
// }


void Caster::set_ray(int x_dcs, int y_dcs, vec3& S, vec3& V) {
    float delta_x = _pixel_width;
    float delta_y = _pixel_height;
    float clip_left = _camera._clip_Left;
    float clip_right = _camera._clip_Right;
    float clip_bottom = _camera._clip_Bottom;
    float clip_near = _camera._clip_Near;
    vec3 eye = _camera._eye;
    vec3 lookat = _camera._lookat;
    vec3 up = _camera._up;

    float x_vcs = clip_left + (x_dcs + 0.5f) * delta_x;
    float y_vcs = clip_bottom + (y_dcs + 0.5f) * delta_y;
    float z_vcs = -clip_near;
    vec3 P_vcs(x_vcs, y_vcs, z_vcs);

    vec3 z_vcs_wcs = normalize(eye - lookat);
    vec3 x_vcs_wcs = normalize(cross(up, z_vcs_wcs));
    vec3 y_vcs_wcs = normalize(cross(z_vcs_wcs, x_vcs_wcs));

    // Transformation matrix to convert from VCS to WCS
    mat4 M_vcs_to_wcs(1.0f);
    M_vcs_to_wcs[0] = vec4(x_vcs_wcs, 0);
    M_vcs_to_wcs[1] = vec4(y_vcs_wcs, 0);
    M_vcs_to_wcs[2] = vec4(z_vcs_wcs, 0);
    M_vcs_to_wcs[3] = vec4(eye, 1.0f);

    vec4 p_wcs_calc = M_vcs_to_wcs * vec4(P_vcs, 1.0f);
    vec3 P_wcs_converted = vec3(p_wcs_calc);

    // Calculate the ray's origin and direction
    S = eye;
    V = normalize(P_wcs_converted - eye);
}

// void Caster::set_ray(int x_dcs, int y_dcs,
//                      vec3& S, vec3& V) {
//     float delta_x = _pixel_width;
//     float delta_y = _pixel_height;
//     float _clip_left = _camera._clip_Left;
//     float _clip_right = _camera._clip_Right;
//     float _clip_bottom = _camera._clip_Bottom;
//     float _clip_near = _camera._clip_Near;
//     vec3 _eye = _camera._eye;
//     vec3 _lookat = _camera._lookat;
//     vec3 _up = _camera._up;

//     float x_vcs = _clip_left + (x_dcs + 0.5) * delta_x;
//     float y_vcs = _clip_bottom + (y_dcs + 0.5) * delta_y;
//     float z_vcs = -(_clip_near);
//     vec3 P_vcs(x_vcs, y_vcs, z_vcs);

//     vec3 z_vcs_wcs = normalize((_eye - _lookat)); //z
//     vec3 x_vcs_wcs = normalize(cross(_up, z_vcs_wcs)); //r
//     vec3 y_vcs_wcs = normalize(cross(z_vcs_wcs, x_vcs_wcs)); //y     

//     // glm::mat4 Model = glm::mat4( 1.0 );
//     // Model[0] = glm::vec4( x_vcs_wcs.x, x_vcs_wcs.y, x_vcs_wcs.z, 0 );
//     // Model[1] = glm::vec4( y_vcs_wcs.x, y_vcs_wcs.y, y_vcs_wcs.z, 0 );
//     // Model[2] = glm::vec4( z_vcs_wcs.x, z_vcs_wcs.y, z_vcs_wcs.z, 1 );
//     // Model[3] = glm::vec4(_eye.x, _eye.y, _eye.z, 1.0 );

//     vec4 p_wcs_calc = vec4(P_vcs, 1.0f);

//     vec4 P_wcs = _M_vcs_to_wcs * p_wcs_calc;
//     vec3 P_wcs_converted = vec3(P_wcs);
// //    vec3 P_wcs_2 = _M_vcs_to_wcs * P_vcs;
//     vec4 eye_convert = vec4(_eye, 1.0f);
// //    vec4 v_calc = P_wcs_converted -eye_convert;
// //    S = vec3(P_wcs_converted);
//     S = P_wcs_converted;
//     vec3 V_calc = glm::normalize(S - _eye);
//     V = V_calc;
//     //V = glm::normalize(vec3(v_calc)); 
// }

// void Caster::set_ray(int x_dcs, int y_dcs, vec3& S, vec3& V) {
//     // Calculate the ray's origin and direction
//     float x_wcs = _camera._clip_Left + (x_dcs + 0.5f) * _pixel_width;
//     float y_wcs = _camera._clip_Bottom + (y_dcs + 0.5f) * _pixel_height;
//     S = _camera._eye; // Ray's start point is the camera's eye position
//     V = glm::normalize(vec3(x_wcs - S.x, y_wcs - S.y, -_camera._clip_Near)); // Ray's direction
// }


void Caster::camera_did_move() {
}

// bool Caster::hits_something(const vec3& start, const vec3& direction) {
//     return false;
// }

bool Caster::hits_something(const vec3& start, const vec3& direction) {
    // Iterate over all shapes in the scene to check for intersections
    for (Shape* s : _scene) {
        Hit dummy_hit;
        if (s->intersects(start, direction, dummy_hit)) {
            return true;
        }
    }
    return false;
}



// bool Caster::get_first_hit(const vec3& start, const vec3& direction,
//                            Hit& hit) {
//     return false;
// }
bool Caster::get_first_hit(const vec3& start, const vec3& direction,
                           Hit& hit) {
    float t = 100000;
    bool state = false;
    for (Shape* s : _scene) {
        Hit curr_hit;
        if (s->intersects(start, direction, curr_hit)) {
            if (curr_hit._t < t) {
                t = curr_hit._t;
                hit = curr_hit;
                state = true;
            }
        }
    }
    return state;
}

// bool Caster::get_first_hit(const vec3& start, const vec3& direction, Hit& hit) {
//     // Initialize hit as nullptr
//     hit.set(vec3(0.0f), nullptr, vec3(0.0f), std::numeric_limits<float>::max());

//     // Iterate over all shapes in the scene
//     for (const auto& shape : _scene) {
//         Hit shape_hit;
//         if (shape->intersects(start, direction, shape_hit) && shape_hit._t < hit._t) {
//             hit = shape_hit;
//         }
//     }

//     // Check if a hit was found
//     return (hit._material != nullptr);
// }



// glm::vec3 Caster::mirror_direction(const vec3& L, const vec3& N) {
//     return vec3(1,0,0);
// }

glm::vec3 Caster::mirror_direction(const vec3& L, const vec3& N) {
    // Calculate the reflection vector R based on L and N
    return 2.0f * glm::dot(N, L) * N - L;
}


// glm::vec3 Caster::local_illumination(const vec3& V, const vec3& N,
//                                      const vec3& L, const vec3& light_color,
//                                      const Material& mat) {
//     return vec3(0.5, 0.5, 0.5);
// }

// glm::vec3 Caster::local_illumination(const vec3& V, const vec3& N,
//                                      const vec3& L, const vec3& light_color,
//                                      const Material& mat) {
//     // Calculate the reflection vector R
//     glm::vec3 R = mirror_direction(L, N);

//     // Calculate the diffuse reflection
//     float kd_dot_N_L = glm::dot(mat._diffuse_reflectance, N) * glm::dot(L, N);

//     // Calculate the specular reflection
//     float ks_R_V_n = glm::pow(glm::dot(R, V), mat._shininess);

//     // Calculate the local illumination using Phong model
//     glm::vec3 illumination = light_color * (kd_dot_N_L + mat._specular_reflectance * ks_R_V_n);

//     return illumination;
// }

// vec3 Caster::local_illumination(const vec3& V, const vec3& N, const vec3& L, const vec3& light_color, const Material& mat) {
//     // Check if the light source is below the surface (N·Li < 0)
//     float NdotL = glm::dot(N, L);
//     if (NdotL < 0.0f) {
//         return vec3(0.0f);
//     }

//     // Calculate the reflection vector R
//     vec3 R = mirror_direction(L, N);

//     // Check if the eye is below the surface (V · N < 0)
//     float VdotN = glm::dot(V, N);
//     if (VdotN < 0.0f) {
//         // Only ambient light
//         return mat._ambient_reflectance * light_color;
//     }

//     // Calculate the diffuse reflection
//     float Idi = NdotL * glm::dot(mat._diffuse_reflectance, L);

//     // Calculate the specular reflection
//     float Isi = glm::pow(glm::dot(R, V), mat._shininess);

//     // Calculate the local illumination using Phong model
//     vec3 illumination = light_color * (Idi + mat._specular_reflectance * Isi);

//     return illumination;
// }


vec3 Caster::local_illumination(const vec3& V, const vec3& N, const vec3& L, const vec3& light_color, const Material& mat) {
    // Calculate the reflection vector R
    vec3 R = mirror_direction(L, N);

    // Ensure the vectors are normalized
    vec3 normalized_L = glm::normalize(L);
    vec3 normalized_R = glm::normalize(R);
    vec3 normalized_N = glm::normalize(N);
    vec3 normalized_V = glm::normalize(V);

    // Initialize the illum/ination components
    vec3 Ia = _ambient_light * mat._ambient_reflectance;
    vec3 Id = vec3(0.0f); // Diffuse component
    vec3 Is = vec3(0.0f); // Specular component

    Hit hit;
    // Calculate diffuse reflection if light is not blocked and below the surface
    if (hits_something(hit._position, normalized_L) || glm::dot(N, L) <= 0.0f) {
        Id = light_color * mat._diffuse_reflectance * glm::max(glm::dot(N, normalized_L), 0.0f);
    }

    // Calculate specular reflection if light is not blocked and below the surface
    if (hits_something(hit._position, normalized_L) || glm::dot(N, L) <= 0.0f) {
        Is = light_color * mat._specular_reflectance * glm::pow(glm::max(glm::dot(normalized_R, normalized_V), 0.0f), mat._shininess);
    }

    // Calculate the total illumination
    vec3 illumination = Ia + Id + Is;

    return illumination;
}



// vec3 Caster::glossy_color(const vec3& S, const vec3& V, const Hit& hit) {
//     return vec3(0.5, 0.5, 0.5);
// }

// vec3 Caster::glossy_color(const vec3& V, const vec3& N, const Hit& hit) {
//     if (hit._material != nullptr) {
//         const Material& mat = *(hit._material);

//         // Initialize the total color
//         vec3 color = vec3(0.0f);

//         // Iterate over the lights in the scene and calculate local illumination
//         for (const Light& light : _lights) {
//             vec3 L = glm::normalize(light._position - hit._position);
//             vec3 light_color = light._color;

//             color += local_illumination(-V, N, L, light_color, mat);
//         }

//         // Add ambient reflection to the color
//         color += mat._ambient_reflectance * _ambient_light;

//         return color;
//     }

//     // If no hit, return a default color (background or black)
//     return _background_color;
// }

vec3 Caster::glossy_color(const vec3& S, const vec3& V, const Hit& hit) {
    if (hit._material != nullptr) {
        const Material& mat = *(hit._material);
        vec3 color = vec3(0.0f); // Initialize the total color

        // Iterate over the lights in the scene and calculate local illumination
        for (const Light& light : _lights) {
            vec3 L = glm::normalize(light._position - hit._position);
            vec3 light_color = light._color;

            color += local_illumination(-V, hit._normal, L, light_color, mat);
        }

        // Add ambient reflection to the color
        color += mat._ambient_reflectance * _ambient_light;

        return color;
    }

    // If no hit, return a default color (background or black)
    return _background_color;
}



// glm::vec3 Caster::glossy_color(const vec3& S, const vec3& V, const Hit& hit) {
//     if (hit._material != nullptr) {
//         Material mat = *(hit._material);

//         // Calculate the normal at the hit point
//         glm::vec3 N = hit._normal;

//         // Initialize the total color
//         glm::vec3 color = glm::vec3(0.0f);

//         // Iterate over the lights in the scene and calculate local illumination
//         for (const Light& light : _lights) {
//             glm::vec3 L = glm::normalize(light._position - hit._position);
//             glm::vec3 light_color = light._color;

//             color += local_illumination(-V, N, L, light_color, mat);
//         }

//         // Add ambient reflection to the color
//         color += mat._ambient_reflectance * _ambient_light;

//         return color;
//     }

    // If no hit, return a default color (background or black)
//     return _background_color;
// }


// vec3 Caster::ray_color(int x_dcs, int y_dcs) {
//     if (x_dcs % 10 == 0 ||
//         y_dcs % 10 == 0)
//         return vec3(0.5, 0.5, 0.5);
//     else
//         return _background_color;/
// }


// vec3 Caster::ray_color(int x_dcs, int y_dcs) {
//     // Calculate the ray's origin and direction
//     vec3 ray_origin = _camera._eye;
//     vec3 ray_direction = calculate_ray_direction(x_dcs, y_dcs);

//     glm::vec3 first_hit_location(0, 0, 0);
//     glm::vec3 second_hit_location(0, 0, 0);

//     // Find the first hit of the ray with the scene
//     Hit hit;

//     set_ray(x_dcs, y_dcs, first_hit_location, second_hit_location);

//     if (get_first_hit(ray_origin, ray_direction, hit)) {
// //    if (get_first_hit(first_hit_location, second_hit_location, hit)) {
//         // Calculate glossy color based on the hit
//         return glossy_color(ray_origin, ray_direction, hit);
// //        return glossy_color(ray_direction, hit._normal, hit);
//     } else {
//         // No hit, return the background color
//         return _background_color;
//     }
// } 

// vec3 Caster::ray_color(int x_dcs, int y_dcs) {
//     // Calculate the ray's origin and direction
//     vec3 ray_origin = _camera._eye;
//     vec3 ray_direction = calculate_ray_direction(x_dcs, y_dcs);

//     // Find the first hit of the ray with the scene
//     Hit hit;
//     if (get_first_hit(ray_origin, ray_direction, hit)) {
//         // Calculate glossy color based on the hit
//         return glossy_color(ray_direction, hit._normal, hit);
//     } else {
//         // No hit, return the background color
//         return _background_color;
//     }
// }

// Calculate the ray color for a pixel
vec3 Caster::ray_color(int x_dcs, int y_dcs) {
    vec3 S, V;
    set_ray(x_dcs, y_dcs, S, V);

    Hit hit;
    if (get_first_hit(S, V, hit)) {
        return glossy_color(S, V, hit);
    } else {
        return _background_color;
    }
}


// vec3 Caster::ray_color(int x_dcs, int y_dcs) {
//     // Calculate the ray's origin and direction
//     vec3 ray_origin = _camera._eye;
//     vec3 ray_direction;
//     set_ray(x_dcs, y_dcs, ray_origin, ray_direction);

//     // Find the first hit of the ray with the scene
//     Hit hit;
//     if (get_first_hit(ray_origin, ray_direction, hit)) {
//         // Calculate glossy color based on the hit
//         vec3 glossy = glossy_color(ray_direction, hit._normal, hit);
//         return glossy;
//     } else {
//         // No hit, return the background color
//         return _background_color;
//     }
// }








// vec3 Caster::ray_color(int x_dcs, int y_dcs) {
//     // Set white pixels on a 10x10 pixel grid.
//     // Obviously you should replace this code!
//     // if (x_dcs % 10 == 0 || y_dcs % 10 == 0)
//     //     return vec3(1, 1, 1);
//     // else
//     //     return _background_color;
//     glm::vec3 first_hit_location(0, 0, 0);
//     glm::vec3 second_hit_location(0, 0, 0);
//     vec3 ray_origin = _camera._eye;
//     vec3 ray_direction = calculate_ray_direction(x_dcs, y_dcs);
//     Hit hit;
//     set_ray(x_dcs, y_dcs, first_hit_location, second_hit_location);
//     if (get_first_hit(ray_origin, ray_direction, hit)) { return glossy_color(ray_origin, ray_direction, hit); } else { return _background_color; }
// }


glm::vec3 Caster::calculate_ray_direction(int x_dcs, int y_dcs) {
    // Calculate the position in camera space
    float u = _camera._clip_Left + x_dcs * _pixel_width;
    float v = _camera._clip_Bottom + y_dcs * _pixel_height;

    // Calculate the ray direction in world coordinates
    glm::vec3 ray_direction = u * _camera._clip_Right + v * _camera._up + _camera._clip_Near * (_camera._lookat - _camera._eye);

    // Normalize the direction vector
    return glm::normalize(ray_direction);
}


/*----------------------------------------------------------------

ray_color (ray, scene) {
hit = first_hit (ray, scene)
obj = hit.shape
return glossy_color (ray, hit, obj, scene)
}
*/

void Caster::read_scene(const string& file_name) {
    Scene_Reader reader;
    try {
        reader.read_scene(file_name, _scene, _camera, _lights);
    }
    catch (invalid_argument& e) {
        cerr << e.what() << endl;
        exit(1);
    }

    _ambient_light = vec3(0, 0, 0);
    for (Light& light : _lights) {
        _ambient_light += light._color * _camera._ambient_fraction;
    }
}

SP_Image Caster::render() {

    // cout << "render" << endl;

    int p = 0;
    for (int y_dcs = 0; y_dcs < _height; y_dcs++) {
        for (int x_dcs = 0; x_dcs < _width; x_dcs++) {

            vec3 color = ray_color(x_dcs, y_dcs);

            int r = static_cast<int>(color.r * 255.0);
            int g = static_cast<int>(color.g * 255.0);
            int b = static_cast<int>(color.b * 255.0);

            r = fmax(0, fmin(r, 255));
            g = fmax(0, fmin(g, 255));
            b = fmax(0, fmin(b, 255));

            _pixels[p++] = r;
            _pixels[p++] = g;
            _pixels[p++] = b;
        }
    }

    vector<unsigned char> image_pixels;
    int num_pixels = _width * _height * 3;
    image_pixels.reserve(num_pixels);
    std::copy(_pixels, _pixels + num_pixels, back_inserter(image_pixels));
    return SP_Image(
        new Image(image_pixels, _width, _height, 3, "Ray cast image"));
}
