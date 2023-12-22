#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp> // glm::to_string()
#include "gl_error.hpp"
#include "program.hpp"

using namespace std;

Shader_Program::Shader_Program() {
}

Shader_Program::~Shader_Program() {
    // cout << "Shader_Program DESTRUCTOR. handle=" << _program_handle << endl;
    glDeleteProgram(_program_handle);
    GL_Error::check("Shader_Program::destructor");
}

Shader_Program::Shader_Program(const string& vertex_shader_source_file,
                               const string& fragment_shader_source_file) {

    _vertex_shader_source   = read_source_file(vertex_shader_source_file);
    _fragment_shader_source = read_source_file(fragment_shader_source_file);
    _vertex_shader_file = vertex_shader_source_file;
    _fragment_shader_file = fragment_shader_source_file;

    install(_vertex_shader_source.c_str(),
            _fragment_shader_source.c_str());
}

string Shader_Program::read_source_file(const string& source_file) const {
    ifstream input_stream;
    input_stream.open(source_file);
    string source_text;
    while (input_stream) {
        string line;
        getline(input_stream, line);
        source_text += line + "\n";
        if (input_stream.eof()) {
            break;
        }
    }

    if (source_text.length() == 0) {
        throw(runtime_error(string("Empty or missing source file: \"")
                            + source_file + "\""));
    }
    input_stream.close();
    return source_text;
}

void Shader_Program::install(const char *vertex_shader_source,
                             const char *fragment_shader_source) {

    // Install the vertex shader
    int vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
    GL_Error::check("CreateShader (vertex shader)");

    // Load the vertex shader's source
    glShaderSource(vertex_shader_handle, 1, &vertex_shader_source, NULL);
    GL_Error::check("ShaderSource (vertex shader)");

    // Compile the vertex shader.
    glCompileShader(vertex_shader_handle);
    GL_Error::check_compiled_OK("vertex shader", vertex_shader_handle,
                                _vertex_shader_file);


    // install fragment shader
    int fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
    GL_Error::check("CreateShader (fragment shader)");

    // Load the fragment shader's source
    glShaderSource(fragment_shader_handle, 1, &fragment_shader_source,
                   NULL);
    GL_Error::check("ShaderSource (fragment shader)");

    // Compile the fragment shader.
    glCompileShader(fragment_shader_handle);
    GL_Error::check_compiled_OK("fragment shader", fragment_shader_handle,
                                _fragment_shader_file);


    // link the two shaders together.
    _program_handle = glCreateProgram();
    GL_Error::check("CreateProgram");

    glAttachShader(_program_handle, vertex_shader_handle);
    GL_Error::check("AttachShader (vertex shader)");

    glAttachShader(_program_handle, fragment_shader_handle);
    GL_Error::check("AttachShader (fragment shader)");

    glLinkProgram(_program_handle);
    GL_Error::check_linked_OK("shader program", _program_handle,
                              _vertex_shader_file,
                              _fragment_shader_file);

    // The two shaders are now joined into a Program.
    // Can delete them.
    glDeleteShader(vertex_shader_handle);
    GL_Error::check("DeleteShader (vertex shader)");

    glDeleteShader(fragment_shader_handle);
    GL_Error::check("DeleteShader (fragment shader)");
}

GLuint Shader_Program::get_handle() const {
    return _program_handle;
}

bool Shader_Program::has_uniform(const string& name) {
    GLint uniform_loc = glGetUniformLocation(_program_handle, name.c_str());
    string message = "Shader_Program::get uniform loc ";
    GL_Error::check(message + name);
    return uniform_loc != -1;
}

GLint Shader_Program::get_uniform_location(const string& name) {
    // First check if we already know this one.
    if (_uniform_location_from_name.find(name) !=
        _uniform_location_from_name.end()) {
        return _uniform_location_from_name[name];
    }

    // We don't.  Ask the driver for it.
    GLint uniform_loc = glGetUniformLocation(_program_handle, name.c_str());
    string message = "Shader_Program::get uniform loc ";
    GL_Error::check(message + name);
    if (uniform_loc == -1) {
        cout << "In program (from \"" << _vertex_shader_file << "\"" << endl;
        cout << "        and      \"" << _fragment_shader_file << "\")" << endl;
        cout << "Uniform variable not found or not used: " << name << endl;
        GL_Error::die_or_continue();
    }
    _uniform_location_from_name[string(name)] = uniform_loc;
    return uniform_loc;
}

// Set a uniform matrix
void Shader_Program::set_mat4(GLint location, const glm::mat4& matrix) const {
    glUseProgram(_program_handle);
    GL_Error::check("Shader::set(mat4)");

    // cout << "Shader_Program::set(" << location << ", a matrix)" << endl;

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    GL_Error::check("Shader::set(matrix)");
}

void Shader_Program::set_mat4(const string& name, const glm::mat4& matrix) {
    GLint location = get_uniform_location(name);
    set_mat4(location, matrix);
}

// Set a uniform vec4
void Shader_Program::set_vec4(GLint location, const glm::vec4& vector) const {
    glUseProgram(_program_handle);
    GL_Error::check("Shader::set(vec4)");

    // cout << "Shader_Program::set(" << location
    //      << " " << glm::to_string(vector) << ")" << endl;
    // cout << *this << endl;

    glUniform4fv(location, 1, glm::value_ptr(vector));
    GL_Error::check("Shader::set(vec4)");
}

void Shader_Program::set_vec4(const string& name, const glm::vec4& vector) {
    GLint location = get_uniform_location(name);
    set_vec4(location, vector);
}

// Set a uniform vec3
void Shader_Program::set_vec3(GLint location, const glm::vec3& vector) const {
    glUseProgram(_program_handle);
    GL_Error::check("Shader::set(vec3)");

    // cout << "Shader_Program::set(" << location
    //      << " " << glm::to_string(vector) << ")" << endl;

    glUniform3fv(location, 1, glm::value_ptr(vector));
    GL_Error::check("Shader::set(vec3)");
}

void Shader_Program::set_vec3(const string& name, const glm::vec3& vector) {
    GLint location = get_uniform_location(name);
    set_vec3(location, vector);
}

// Set a uniform vec2
void Shader_Program::set_vec2(GLint location, const glm::vec2& vector) const {
    glUseProgram(_program_handle);
    GL_Error::check("Shader::set(vec2)");

    glUniform2fv(location, 1, glm::value_ptr(vector));
    GL_Error::check("Shader::set(vec2)");
}

void Shader_Program::set_vec2(const string& name, const glm::vec2& vector) {
    GLint location = get_uniform_location(name);
    set_vec2(location, vector);
}

// Set a uniform float
void Shader_Program::set_float(GLint location, float value) const {
    glUseProgram(_program_handle);
    GL_Error::check("Shader::set(float)");

    glUniform1f(location, value);
    GL_Error::check("Shader::set(float)");
}

void Shader_Program::set_float(const string& name, float value) {
    GLint location = get_uniform_location(name);
    set_float(location, value);
}

// Set a uniform int
void Shader_Program::set_int(GLint location, int value) const {
    glUseProgram(_program_handle);
    GL_Error::check("Shader::set(int)");

    glUniform1i(location, value);
    GL_Error::check("Shader::set(int)");
}

void Shader_Program::set_int(const string& name, int value) {
    GLint location = get_uniform_location(name);
    set_int(location, value);
}

ostream& operator<<(ostream& os, const Shader_Program& prog) {
    os << "Shader Program (handle = " << prog._program_handle << ")" << endl;
    os << "Vertex shader source: " << endl;
    os << prog._vertex_shader_source << endl;
    os << "Fragment shader source: " << endl;
    os << prog._fragment_shader_source << endl;
    return os;
}

