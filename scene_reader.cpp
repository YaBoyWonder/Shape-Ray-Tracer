#include "scene_reader.hpp"
#include <iostream>

using std::cout;
using std::endl;

Scene_Reader::Scene_Reader() {
}

vec3 Scene_Reader::read_vec3(Tokenizer& tokens) {
    float x = tokens.next_number();
    float y = tokens.next_number();
    float z = tokens.next_number();
    return vec3(x, y, z);
}

Sphere *Scene_Reader::read_sphere(Tokenizer& tokens, vector<Material>& materials) {
    vec3 center(0,0,0);
    float radius=1;
    string name = "NO NAME";
    Material mat;
    string token;
    while ((token = tokens.next_string()) != "end") {
        if (token == "center")
            center = read_vec3(tokens);
        else if (token == "radius")
            radius = tokens.next_number();
        else if (token == "name")
            name = tokens.next_string();
        else if (token == "material")
            mat = find_named_material(tokens.next_string(), materials);
    }
    match("sphere", tokens);
    return new Sphere(center, radius, mat, name);
}

Triangle* Scene_Reader::read_triangle(Tokenizer& tokens, vector<Material>& materials) {
    vec3 A(0,0,0);
    vec3 B(1,0,0);
    vec3 C(0,2,0);
    string name = "NO NAME";
    Material mat;
    string token;
    while ((token = tokens.next_string()) != "end") {
        if (token == "end")
            break;
        if (token == "a")
            A = read_vec3(tokens);
        else if (token == "b")
            B = read_vec3(tokens);
        else if (token == "c")
            C = read_vec3(tokens);
        else if (token == "material")
            mat = find_named_material(tokens.next_string(), materials);
        else if (token == "name")
            name = tokens.next_string();
    }

    match("triangle", tokens);
    return new Triangle(A, B, C, mat, name);
}

Cylinder* Scene_Reader::read_cylinder(Tokenizer& tokens, vector<Material>& materials) {
    vec3 center(0,0,0);
    float radius=1;
    float height=2;
    string name = "NO NAME";
    Material mat;
    string token;
    while ((token = tokens.next_string()) != "end") {
        if (token == "center")
            center = read_vec3(tokens);
        else if (token == "radius")
            radius = tokens.next_number();
        else if (token == "height")
            height = tokens.next_number();
        else if (token == "name")
            name = tokens.next_string();
        else if (token == "material")
            mat = find_named_material(tokens.next_string(), materials);
    }

    match("cylinder", tokens);
    return new Cylinder(center, radius, height, mat, name);
}

Light Scene_Reader::read_light(Tokenizer& tokens) {
    Light light;
    string token;
    while ((token = tokens.next_string()) != "end") {
        if (token == "end")
            break;
        if (token == "position")
            light._position = read_vec3(tokens);
        else if (token == "color")
            light._color = read_vec3(tokens);
        else if (token == "name")
            light._name = tokens.next_string();
    }
    match("light", tokens);
    return light;
}

Material Scene_Reader::find_named_material(const string name,
                                           const vector<Material>& materials) {
    for (const Material& mat : materials) {
        if (mat._name == name)
            return mat;
    }
    throw invalid_argument("Can't find material named \"" + name + "\"");
}

Material Scene_Reader::read_material(Tokenizer& tokens) {
    Material mat;
    string token;
    while ((token = tokens.next_string()) != "end") {
        if (token == "ambient")
            mat._ambient_reflectance = read_vec3(tokens);
        else if (token == "diffuse")
            mat._diffuse_reflectance = read_vec3(tokens);
        else if (token == "specular")
            mat._specular_reflectance = read_vec3(tokens);
        else if (token == "shininess")
            mat._shininess = tokens.next_number();
        else if (token == "name")
            mat._name = tokens.next_string();
    }
    match("material", tokens);
    return mat;
}

Camera Scene_Reader::read_camera(Tokenizer& tokens) {
    Camera camera;
    string token;
    while ((token = tokens.next_string()) != "end") {
        if (token == "eye")
            camera._eye = read_vec3(tokens);
        else if (token == "lookat")
            camera._lookat = read_vec3(tokens);
        else if (token == "vup")
            camera._up = read_vec3(tokens);
        else if (token == "clip") {
            camera._clip_Left = tokens.next_number();
            camera._clip_Right = tokens.next_number();
            camera._clip_Bottom = tokens.next_number();
            camera._clip_Top = tokens.next_number();
            camera._clip_Near = tokens.next_number();
        } else if (token == "ambient_fraction")
            camera._ambient_fraction = tokens.next_number();
    }
    match("camera", tokens);
    camera.init_basis();
    return camera;
}


void Scene_Reader::match(const string& expected, Tokenizer& tokens) {
    string token = tokens.next_string();
    if (token != expected) {
        throw invalid_argument("Expected \"" + expected + "\", got \""
                               + token + "\" "
                               + tokens.file_position());
    }
}


void Scene_Reader::read_scene(const string& file_name,
                              vector<Shape*>& shapes,
                              Camera& camera,
                              vector<Light>& lights) {
    Tokenizer tokens(file_name);
    vector<Material> materials;
    while (!tokens.eof()) {
        string token = tokens.next_string();
        if (tokens.eof())
            break;
        if (token == "begin") {
            string kind = tokens.next_string();
            if (kind == "material")
                materials.push_back(read_material(tokens));
            else if (kind == "camera")
                camera = read_camera(tokens);
            else if (kind == "light")
                lights.push_back(read_light(tokens));
            else if (kind == "triangle")
                shapes.push_back(read_triangle(tokens, materials));
            else if (kind == "sphere")
                shapes.push_back(read_sphere(tokens, materials));
            else if (kind == "cylinder")
                shapes.push_back(read_cylinder(tokens, materials));
        }
        else {
            throw invalid_argument("Expected \"begin\", got \""
                                   + token + "\"\n"
                                   + " on " + tokens.file_position());
        }
    }
}
