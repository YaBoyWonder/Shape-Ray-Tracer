#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include "gl_error.hpp"

using namespace std;

// Initialize the static variable.
bool GL_Error::die_on_error = true;

void GL_Error::check(const string& msg) {
    GLenum error = glGetError();
    switch (error) {
    case GL_NO_ERROR:
        return;

    case GL_INVALID_ENUM:
        cout << "Error: GL_INVALID_ENUM on " << msg << "\n";
        break;

    case GL_INVALID_VALUE:
        cout << "Error: GL_INVALID_VALUE on " << msg << "\n";
        break;

    case GL_INVALID_OPERATION:
        cout << "Error: GL_INVALID_OPERATION on " << msg << "\n";
        break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        cout << "Error: GL_INVALID_FRAMEBUFFER_OPERATION on " << msg << "\n";
        break;

    case GL_OUT_OF_MEMORY:
        cout << "Error: GL_OUT_OF_MEMORY on " << msg << "\n";
        break;

    case GL_STACK_UNDERFLOW:
        cout << "Error: GL_STACK_UNDERFLOW on " << msg << "\n";
        break;

    case GL_STACK_OVERFLOW:
        cout << "Error: GL_STACK_OVERFLOW on " << msg << "\n";
        break;

    default:
        cout << "Unknown error on " << msg << "\n";
        break;
    }

    die_or_continue();
}

void GL_Error::die_or_continue(const string& message) {
    cout << message << endl;
    cout.flush();
    if (die_on_error) {
        cout << "This is fatal because GL_Error::die_on_error==true)" << endl;
        cout.flush();
        string reply;
        cout << "Press Control-C (program will terminate)";
        std::cin >> reply;
        throw runtime_error(message);
    }
}

void GL_Error::check_compiled_OK(const string& msg,
                                 int shader_handle,
                                 const string& file_name) {
    const int BUFSIZE = 1024;
    int success;
    char info_log[BUFSIZE];
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_handle, BUFSIZE, NULL, info_log);
        cout << "Compilation failed: " << msg << endl;
        cout << info_log << endl;
        cout << "Source file: " << file_name << endl;
        die_or_continue();
    }
}

void GL_Error::check_linked_OK(const string& msg,
                               int program_handle,
                               const string& vertex_shader_file,
                               const string& fragment_shader_file) {
    const int BUFSIZE = 1024;
    int success;
    char info_log[BUFSIZE];
    // check for linking errors
    glGetProgramiv(program_handle, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_handle, BUFSIZE, NULL, info_log);
        cout << "Linking failed: " << msg << endl;
        cout << info_log << endl;
        cout << "Vertex shader file: " << vertex_shader_file << endl;
        cout << "Fragment shader file: " << fragment_shader_file << endl;
        die_or_continue();
    }
}

// void GL_Error::gl_error_callback(GLenum source,
//                                  GLenum type,
//                                  GLuint id,
//                                  GLenum severity,
//                                  GLsizei length,
//                                  const GLchar* message,
//                                  const void* userParam){
//     std::cerr << "GL CALLBACK: type = " << type
//         //  << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" )
//               << " type = " << type
//               << " severity = " << severity
//               << " message = " << message << endl;
// }
