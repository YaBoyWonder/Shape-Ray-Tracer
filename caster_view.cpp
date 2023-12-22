#include "caster_view.hpp"
#include "program.hpp"
#include "vertex_buffer.hpp"
#include "element_buffer.hpp"
#include "vertex_array.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

using glm::vec2;
using glm::vec3;
using std::vector;

Caster_View::Caster_View() {
    vector<int> attribute_dims{3, 2};

    // Make a square that goes from (-1,-1) to (+1,+1),
    // onto which we'll paint a texture.

    SP_Vertex_Buffer vertices(
        new Vertex_Buffer(4, attribute_dims, "Square vertices"));

    const int POSITION = 0;
    const int TEXTURE_COORD = 1;

    vertices->add(POSITION,      vec3(-1, -1, 0));
    vertices->add(TEXTURE_COORD, vec2( 0,  0));

    vertices->add(POSITION,      vec3(+1, -1, 0));
    vertices->add(TEXTURE_COORD, vec2( 1,  0));

    vertices->add(POSITION,      vec3(+1, +1, 0));
    vertices->add(TEXTURE_COORD, vec2( 1,  1));

    vertices->add(POSITION,      vec3(-1, +1, 0));
    vertices->add(TEXTURE_COORD, vec2( 0,  1));

    vertices->install_data();

    SP_Element_Buffer indexes(
        new Element_Buffer(vector<int>{0, 1, 2, 3},
                           "Square Indexes"));
    indexes->install_data();

    SP_Vertex_Array varray(
        new Vertex_Array(vertices,
                         indexes,
                         GL_TRIANGLE_FAN,
                         "Square Vertex Array"));
    varray->install_data();

    SP_Shader_Program program(
        new Shader_Program("shaders/pass_through_xyz_uv.vs",
                           "shaders/texture_plain.fs"));

    _square_mesh = SP_Mesh(new Mesh(program, "Square", varray));

    // In the fragment shader, attach sampler to
    // the first (and only) texture unit.
    program->set_int("current_texture", 0);
}

void Caster_View::draw(SP_Image image) {

    // cout << "Gonna draw" << endl;

    glDisable(GL_DEPTH_TEST);

    glClearColor(0.8f, 0.8f, 0.7f, 1.0f); // very light beige
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make a texture from the image.
    _texture = SP_Texture(new Texture(image));

    // bind texture to its unit
    _texture->attach_texture_unit(0);

    _square_mesh->draw();

    // cout << "Drew this mesh: " << *_square_mesh << endl;
}
