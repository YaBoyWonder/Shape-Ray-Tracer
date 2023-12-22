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

using glm::max;
#define EPSILON 0.001

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


void Caster::set_ray(int x_dcs, int y_dcs, vec3& S, vec3& V) {
    float delta_x = _pixel_width;
    float delta_y = _pixel_height;
    float clip_left = _camera._clip_Left;
    //float clip_right = _camera._clip_Right;
    float clip_bottom = _camera._clip_Bottom;
    float clip_near = _camera._clip_Near;
   
    vec3 eye = _camera._eye;
    //vec3 lookat = _camera._lookat;
    //vec3 up = _camera._up;

    float x_vcs = clip_left + (x_dcs + 0.5f) * delta_x;
    float y_vcs = clip_bottom + (y_dcs + 0.5f) * delta_y;
    float z_vcs = -clip_near;
    vec3 P_vcs(x_vcs, y_vcs, z_vcs);

    //vec3 z_vcs_wcs = normalize(eye - lookat);
    //vec3 x_vcs_wcs = normalize(cross(up, z_vcs_wcs));
    //vec3 y_vcs_wcs = normalize(cross(z_vcs_wcs, x_vcs_wcs));

    // Transformation matrix to convert from VCS to WCS
    // mat4 M_vcs_to_wcs(1.0f);
    // M_vcs_to_wcs[0] = vec4(x_vcs_wcs, 0);
    // M_vcs_to_wcs[1] = vec4(y_vcs_wcs, 0);
    // M_vcs_to_wcs[2] = vec4(z_vcs_wcs, 0);
    // M_vcs_to_wcs[3] = vec4(eye, 1.0f);

    vec4 p_wcs_calc = _M_vcs_to_wcs * vec4(P_vcs, 1.0f);
    vec3 P_wcs_converted = vec3(p_wcs_calc);

    // Calculate the ray's origin and direction
    S = P_wcs_converted;
    V = normalize(P_wcs_converted - eye);

}


void Caster::camera_did_move() {
    vec3 _eye = _camera._eye;
    vec3 _up = _camera._up; 
    vec3 _lookat = _camera._lookat;
    vec3 z_vcs_wcs = normalize((_eye - _lookat)); //z
    vec3 x_vcs_wcs = normalize(cross(_up, z_vcs_wcs)); //r
    vec3 y_vcs_wcs = normalize(cross(z_vcs_wcs, x_vcs_wcs)); //y     
    _M_vcs_to_wcs[0] = glm::vec4( x_vcs_wcs.x, x_vcs_wcs.y, x_vcs_wcs.z, 0 );
    _M_vcs_to_wcs[1] = glm::vec4( y_vcs_wcs.x, y_vcs_wcs.y, y_vcs_wcs.z, 0 );
    _M_vcs_to_wcs[2] = glm::vec4( z_vcs_wcs.x, z_vcs_wcs.y, z_vcs_wcs.z, 1 );
    _M_vcs_to_wcs[3] = glm::vec4(_eye.x, _eye.y, _eye.z, 1.0 );
}


bool Caster::hits_something(const vec3& start, const vec3& direction) {
    for (Shape* s : _scene) {
        Hit hit;
        if (s->intersects(start, direction, hit)) {
            float t = dot(hit._position - start, direction);
            if (t > 0.001f) { return true; }
        }
    }
    return false;
}





/*
TRAVERSAL:
first_hit(ray, cell) {

    if (ray misses cell):
        return
    
    if (cell is a leaf):
        for (each shape in cell) {
            if (ray hits shape inside cell):
                save that hit
        }
        return closest hit
    }
    else {

        for (each child cell in order that ray vists children):
            return first_hit(ray, child)
    
    }
}
*/

bool Caster::get_first_hit(const vec3& start, const vec3& direction, Hit& hit) {
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



glm::vec3 Caster::mirror_direction(const vec3& L, const vec3& N) { return 2.0f * dot(N, L) * N - L; }


vec3 Caster::local_illumination(const vec3& V, const vec3& N, const vec3& L, const vec3& light_color, const Material& mat) {
    vec3 N_2 = normalize(N);
    vec3 L_Dir = normalize(L);
    vec3 V_2  = normalize(V);
    vec3 R = mirror_direction(L_Dir, N_2);
    vec3 kd = mat._diffuse_reflectance;
    vec3 ks = mat._specular_reflectance;
    float n = mat._shininess;
    float ambient_strength = 0.001f;
    //float ambient_strength = 0.1;
    //float specularStrength = 0.5;
    float spec = pow(max(dot(V_2, R), 0.001f), n);

    //vec3 ambient = ambient_strength * light_color * mat._ambient_reflectance;
    vec3 ambient = ambient_strength * light_color * mat._ambient_reflectance;
    vec3 diffuse = max(dot(N_2, L_Dir), 0.001f) * light_color * kd;
    vec3 specular = spec * light_color * ks;
    vec3 output = (ambient + diffuse + specular);
    return output;
}


vec3 Caster::glossy_color(const vec3& S, const vec3& V, const Hit& hit) {
    if (hit._material != nullptr) {
        vec3 color = glm::vec3(0, 0, 0);
            for (auto& light : _lights) {
                vec3 L = normalize(light._position - hit._position);
                if (_shadowing && hits_something(hit._position, L)) { continue; }
                color += local_illumination(-V, hit._normal, normalize(light._position - hit._position), light._color, *hit._material);
            }
        color += hit._material->_ambient_reflectance * _ambient_light;
        return color;
    } else { return _background_color; }
}


vec3 Caster::ray_color(int x_dcs, int y_dcs) {
    vec3 S, V;
    Hit hit;
    set_ray(x_dcs, y_dcs, S, V);
    if (get_first_hit(S, V, hit)) { return glossy_color(S, V, hit); }
    else { return _background_color; }
} 


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