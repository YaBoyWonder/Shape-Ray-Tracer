#ifndef _SCENE_READER_HPP
#define _SCENE_READER_HPP

#include "tokenizer.hpp"
#include "shape.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "triangle.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"
#include <vector>
#include <string>
#include <exception>
#include <glm/vec3.hpp>

using std::string;
using std::invalid_argument;
using std::vector;
using glm::vec3;

class Scene_Reader {
 public:
    Scene_Reader();
    void read_scene(const string& file_name,
                    vector<Shape*>& shapes,
                    Camera& camera,
                    vector<Light>& lights);

 private:
    Sphere *read_sphere(Tokenizer& tokens, vector<Material>& materials);
    Triangle *read_triangle(Tokenizer& tokens, vector<Material>& materials);
    Cylinder *read_cylinder(Tokenizer& tokens, vector<Material>& materials);
    Light read_light(Tokenizer& tokens);
    Material find_named_material(const string name,
                                 const vector<Material>& materials);
    vec3 read_vec3(Tokenizer& tokens);
    Material read_material(Tokenizer& tokens);
    Camera read_camera(Tokenizer& tokens);
    void match(const string& expected, Tokenizer& tokens);
};

#endif
