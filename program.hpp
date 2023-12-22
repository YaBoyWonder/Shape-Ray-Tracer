#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>
#include <map>
#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glad/glad.h>

using namespace std;

/** A wrapper around a vertex shader, plus a fragment shader.
 *
 * Abstracts some of the openGL function calls, while exposing
 * many of the features of shader programs.
 */
class Shader_Program {
public:

    /** Default constructor, to keep compiler happy.
     */
    Shader_Program();

    /** Parameterized constructor.
     *
     * You should pass it the names of the two files that hold
     * the source code for the vertex shader and fragment shader.
     * This will call install() to actually create the shader program
     * on the GPU.
     *
     * @param vertex_shader_file The name of the text file that holds
     *                           the glsl code for the vertex shader.
     *                           Typically it will have a .vs file extension.
     * @param fragment_shader_file The name of the text file that holds
     *                             the glsl code for the fragment shader.
     *                             Typically it will have a .fs file extension.
     */
    Shader_Program(const string& vertex_shader_file,
                   const string& fragment_shader_file);

    /** Destructor.
     */
    ~Shader_Program();

    /** Install the shader program.
     *
     * You should pass it two C strings, which are
     * the source code for the two shaders.
     *
     * This calls the following openGL functions for you:
```
glCreateShader  : creates a vertex shader or a fragment shader.
glShaderSource  : loads the code for the shader.
glCompileShader : compiles the shader, and checks for errors.
glCreateProgram : creates a shader program (vertex shader + fragment shader).
glAttachShader  : attach a vertex or fragment shader to the program.
glLinkProgram   : links vertex + fragment shaders, and checks for errors.
glDeleteShader  : deletes a vertex or fragment shader (after they're linked).
```
     * @param vertex_shader_source The glsl code for the vertex shader.
     * @param fragment_shader_source The glsl code for the fragment shader.
     */

    void install(const char *vertex_shader_source,
                 const char *fragment_shader_source);

    /** Read a whole text file from disk.
     *
     * @param source_file The name of the text file.
     * @return The contents of the text file, in one big string
     */
    string read_source_file(const string& source_file) const;

    /** Return openGL handle for the shader program (an int).
     *
     * This is needed by Vertex_Array::draw(), to tell openGL which
     * program to use to draw the shapes.
     *
     * @return The handle.
     */
    GLuint get_handle() const;

    /** Query shader whether a given uniform exists.
     *
     * @param name The name of the uniform variable.
     * @return whether it exists.
     */
    bool has_uniform(const string& name);

    /** Access a uniform (global) variable's location in the program.
     *
     * @param name The name of the uniform variable.
     * @return The location of the variable, in the shader program.
     */
    GLint get_uniform_location(const string& name);

    /** Set the values of a uniform matrix.
     *
     * @param location The location of the matrix's uniform variable.
     * @param matrix The matrix values.
     */
    void set_mat4(GLint location, const glm::mat4& matrix) const;

    /** Set the values of a uniform matrix.
     *
     * This will call get_uniform_location() to query openGL
     * and get the location for the given name.  It's more efficient
     * to call get_uniform_location() yourself once in your program,
     * and call the other version of this function instead.
     *
     * @param name The name of the matrix's uniform variable.
     * @param matrix The matrix values.
     */
    void set_mat4(const string& name, const glm::mat4& matrix);

    /** Set the values of a uniform vec4.
     *
     * @param location The location of the vector's uniform variable.
     * @param vector The vector.
     */
    void set_vec4(GLint location, const glm::vec4& vector) const;

    /** Set the values of a uniform vec4.
     *
     * This will call get_uniform_location() to query openGL
     * and get the location for the given name.  It's more efficient
     * to call get_uniform_location() yourself once in your program,
     * and call the other version of this function instead.
     *
     * @param name The name of the vector's uniform variable.
     * @param vector The vector.
     */
    void set_vec4(const string& name, const glm::vec4& vector);

    /** Set the values of a uniform vec3.
     *
     * @param location The location of the vector's uniform variable.
     * @param vector The vector.
     */
    void set_vec3(GLint location, const glm::vec3& vector) const;

    /** Set the values of a uniform vec3.
     *
     * This will call get_uniform_location() to query openGL
     * and get the location for the given name.  It's more efficient
     * to call get_uniform_location() yourself once in your program,
     * and call the other version of this function instead.
     *
     * @param name The name of the vector's uniform variable.
     * @param vector The vector.
     */
    void set_vec3(const string& name, const glm::vec3& vector);

    /** Set the values of a uniform vec3.
     *
     * @param location The location of the vector's uniform variable.
     * @param vector The vector.
     */
    void set_vec2(GLint location, const glm::vec2& vector) const;

    /** Set the values of a uniform vec2.
     *
     * This will call get_uniform_location() to query openGL
     * and get the location for the given name.  It's more efficient
     * to call get_uniform_location() yourself once in your program,
     * and call the other version of this function instead.
     *
     * @param name The name of the vector's uniform variable.
     * @param vector The vector.
     */
    void set_vec2(const string& name, const glm::vec2& vector);

    /** Set the value of a uniform float.
     *
     * @param location The location of the float's uniform variable.
     * @param value The value.
     */
    void set_float(GLint location, float value) const;

    /** Set the value of a uniform float.
     *
     * This will call get_uniform_location() to query openGL
     * and get the location for the given name.  It's more efficient
     * to call get_uniform_location() yourself once in your program,
     * and call the other version of this function instead.
     *
     * @param name The name of the float's uniform variable.
     * @param value The value
     */
    void set_float(const string& name, float value);

    /** Set the value of a uniform int.
     *
     * @param location The location of the int's uniform variable.
     * @param value The value.
     */
    void set_int(GLint location, int value) const;

    /** Set the value of a uniform int.
     *
     * This will call get_uniform_location() to query openGL
     * and get the location for the given name.  It's more efficient
     * to call get_uniform_location() yourself once in your program,
     * and call the other version of this function instead.
     *
     * @param name The name of the int's uniform variable.
     * @param value The value
     */
    void set_int(const string& name, int value);

    /** Output to a stream, used for debugging.
     */
    friend ostream& operator<<(ostream& os, const Shader_Program& prog);

private:
    GLuint _program_handle;
    string _vertex_shader_source;
    string _fragment_shader_source;
    string _vertex_shader_file;
    string _fragment_shader_file;
    map<string, GLint> _uniform_location_from_name;
};

typedef shared_ptr<Shader_Program> SP_Shader_Program;

#endif
