#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include "gl_error.hpp"
#include "vertex_array.hpp"

Vertex_Array::Vertex_Array() {
}

Vertex_Array::~Vertex_Array() {
    // cout << "VERTEX ARRAY DESTRUCTOR! " << _name
    //      << " @" << this
    //      << " (handle " << _array_handle << ")" << endl;

    destroy();
}

Vertex_Array::Vertex_Array(SP_Vertex_Buffer vertex_buffer,
                           SP_Element_Buffer element_buffer,
                           GLuint draw_mode,
                           const std::string& vertex_array_name)
    : _draw_mode(draw_mode),
      _vertex_buffer(vertex_buffer),
      _element_buffer(element_buffer),
      _name(vertex_array_name)
{
    // cout << "Vertex_Array::ctor.  Gonna install data." << endl;

    install_data();

    //cout << "After installing data, vertex array @" << this << " is:" << endl;
    // cout << *this << endl;
}

void Vertex_Array::destroy() const {

    // cout << "Destroying " << *this << endl;

    glDeleteVertexArrays(1, &_array_handle);
    GL_Error::check("Vertex_Array destructor. DeleteVertexArrays");
}

void Vertex_Array::install_data() {

    // cout << "Vertex_Array::install_data." << endl;
    // cout << "  vertices: " << _vertex_buffer << endl;
    // cout << "  elements: " << _element_buffer << endl;

    glGenVertexArrays(1, &_array_handle);
    GL_Error::check("Vertex_Array ctor. GenVertexArrays");

    // cout << "Vertex_Array::install_data. Got array handle "
    //      << _array_handle << endl;

    // Activate the vertex array we just created.
    glBindVertexArray(_array_handle);
    GL_Error::check("Vertex_Array ctor. BindVertexArray");

    // Attach to it the vertex buffer and the element buffer
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer->get_handle());
    GL_Error::check("Vertex_Array ctor. BindBuffer (vertex buffer)");

    _vertex_buffer->define_attributes();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_buffer->get_handle());
    GL_Error::check("Vertex_Array ctor. BindBuffer (element buffer)");

    // The vertex buffer and element buffer are now attached to
    // this vertex array buffer.
    // Now we can unbind the vertex array buffer
    glBindVertexArray(0);
    GL_Error::check("Vertex_Array ctor. un-BindBuffer(vertex array)");
}

void Vertex_Array::draw(SP_Shader_Program program) const {

    // cout << "Vertex_Array::draw.  Array @" << this << " is" << endl;
    // cout << *this << endl;

    glUseProgram(program->get_handle());
    GL_Error::check("Vertex_Array::draw. UseProgram");

    glBindVertexArray(_array_handle);
    GL_Error::check("Vertex_Array::draw. BindVertexArray");

    // cout << "glDrawElements. name = " << _name
    //      << " draw mode = "<< draw_mode_name(_draw_mode) << endl;

    glDrawElements(_draw_mode, _element_buffer->get_size(),
                   GL_UNSIGNED_INT, 0);
    GL_Error::check("Vertex_Array::draw. DrawElements");
}

glm::vec4 Vertex_Array::get(int attribute, int index) const {
    return _vertex_buffer->get(attribute, index);
}

GLint Vertex_Array::get_draw_mode() const {
    return _draw_mode;
}

int Vertex_Array::num_vertices() const {
    return _vertex_buffer->get_num_vertices();
}

int Vertex_Array::num_elements() const {
    return _element_buffer->get_size();
}

GLuint Vertex_Array::get_element(int index) const {
    return _element_buffer->get(index);
}

SP_Vertex_Buffer Vertex_Array::get_vertices() const {
    return _vertex_buffer;
}

SP_Element_Buffer Vertex_Array::get_indexes() const {
    return _element_buffer;
}

ostream& operator<<(ostream& os, const Vertex_Array& array) {
    os << "Vertex_Array \"" << array._name << "\"\n";
    os << "   handle: "   << array._array_handle
       << " #elements: "  << array._element_buffer->get_size()
       << " draw_mode: "
       << Vertex_Array::draw_mode_name(array._draw_mode) << endl;
    os << "   vertex buffer: " << *(array._vertex_buffer) << endl;
    os << "   element buffer: " << *(array._element_buffer) << endl;
    return os;
}

GLuint Vertex_Array::get_handle() const {
    return _array_handle;
}

string Vertex_Array::draw_mode_name(GLuint draw_mode) {
    static map<GLuint, string> names {
        {GL_POINTS, "GL_POINTS"},
        {GL_LINE_STRIP, "GL_LINE_STRIP"},
        {GL_LINE_LOOP, "GL_LINE_LOOP"},
        {GL_LINES, "GL_LINES"},
        {GL_LINE_STRIP_ADJACENCY, "GL_LINE_STRIP_ADJACENCY"},
        {GL_LINES_ADJACENCY, "GL_LINES_ADJACENCY"},
        {GL_TRIANGLE_STRIP, "GL_TRIANGLE_STRIP"},
        {GL_TRIANGLE_FAN, "GL_TRIANGLE_FAN"},
        {GL_TRIANGLES, "GL_TRIANGLES"},
        {GL_TRIANGLE_STRIP_ADJACENCY, "GL_TRIANGLE_STRIP_ADJACENCY"},
        {GL_TRIANGLES_ADJACENCY, "GL_TRIANGLES_ADJACENCY"}
    };
    if (names.find(draw_mode) == names.end()) {
        stringstream os;
        os << draw_mode << " (unknown)";
        return os.str();
    }
    else {
        return names[draw_mode];
    }
}

