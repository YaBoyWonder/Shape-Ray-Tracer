#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <string>
#include "program.hpp"
#include "vertex_buffer.hpp"
#include "element_buffer.hpp"

/**
 * This class implements an "indexed face set", which in openGL-speak
 * is a Vertex Array.
 *
 * It's constructed from a Vertex_Buffer (a collection of vertices),
 * and an Element_Buffer (a collection of indexes into those vertices).
 * The indexes tell the GPU how to connect the vertices.
 *
 * To actually draw the mesh, you need a Shader object to process the vertices
 * and pixels.
 *
 * Example:
```
// Make and populate the vertex buffer for a triangle.
GLint Position = 0;        // First (and only) vertex attribute at index 0.
vector<int> dimensions{3}; // Vertex attribute has 3 floats.
SP_Vertex_Buffer vb(new Vertex_Buffer(3, dimensions, "My Triangle Vertices"));
vb->add(Position, 0, 0, 0);
vb->add(Position, 1, 0, 0);
vb->add(Position, 1, 1, 0);
vb->install_data();

// Make and populate the element buffer
SP_Element_Buffer eb(new Element_Buffer(3, "My triangle indexes"));
eb->add( 0 );
eb->add( 1 );
eb->add( 2 );
eb->install_data();

// And finally make the Vertex_Array.
SP_Vertex_Array triangle(new Vertex_Array(vb, eb, GL_TRIANGLES, "My Triangle"));

// and draw it.
triangle->draw(<pointer to the Shader here>);
```
 */

class Vertex_Array {
 public:

    /** Default constructor, to keep the compiler happy. */
    Vertex_Array();
    /** Destructor */
    ~Vertex_Array();

    /** Parameterized constructor.
     * @param vertex_buffer The vertex information.
     * @param element_buffer Indexes into the vertex buffer.
     * @param draw_mode How to connect the vertices.
     * @param vertex_array_name optional name
     */
    Vertex_Array(SP_Vertex_Buffer vertex_buffer,
                 SP_Element_Buffer element_buffer,
                 GLuint draw_mode,
                 const std::string& vertex_array_name="NO NAME");

    /** Connect together the vertex buffer and element buffer
     * into a vertex array, on the GPU.
     *
     * This hides the following OpenGL functions from the client:
```
glGenVertexArrays() : Make a Vertex Array object on the GPU.
glBindVertexArray() : Activate the vertex array on the GPU.
glBindBuffer()      : Activate the vertex buffer and/or the element buffer.
```
     * It also calls these functions (indirectly through `Vertex_Buffer::define_attributes`)`
```
glEnableVertexAttribArray() : Activate one set of vertex attributes
glVertexAttribPointer()     : Tell the GPU where one set of vertex attributes
                              is stored in our CPU buffer.
```
     */
    void install_data();

    /** Delete the vertex array from the GPU.
     */
    void destroy() const;

    /** Tell openGL to draw the vertex_array.
     * @param shader The vertex shader + fragment shader.
     */
    void draw(SP_Shader_Program shader) const;

    /** Retrieve one vertex datum.
     *
     * This is just a wrapper around Vertex_Buffer::get().
     *
     * @param attribute Which attribute of the vertex to retrieve.
     * @param index Which vertex.
     * @return 4 values for the vertex, in a vec4.
     */
    glm::vec4 get(int attribute, int index) const;

    /** Retrieve one element index.
     *
     * This is just a wrapper around Element_Buffer::get().
     * @param index Which index.
     */
    GLuint get_element(int index) const;

    /** Access the draw mode.
     * @return the draw mode
     */
    GLint get_draw_mode() const;

    /** Access the vertex count.
     * @return how many vertices there are.
     */
    int num_vertices() const;

    /** Access the number of indexes.
     * @return how many indexes there are.
     */
    int num_elements() const;

    /** Vertex_Array's name (optional, for debugging)
     * @return the name
     */
    std::string _name;

    /** Access the vertex buffer.
     * @return The vertex buffer.
     */
    SP_Vertex_Buffer get_vertices() const;

    /** Access the element buffer.
     * @return The element buffer.
     */
    SP_Element_Buffer get_indexes() const;

    /** Output to stream (for debugging)
     */
    friend ostream& operator<<(ostream& os, const Vertex_Array& array);

    /** User-friendly names for drawing modes
     */
    static string draw_mode_name(GLuint draw_mode);

    /** Access the handle.
     * @return the Vertex Array's object.
     */
    GLuint get_handle() const;

 private:
    /** OpenGL's handle for this vertex array.  AKA a "vertex array object" */
    GLuint _array_handle;
    /** Draw mode used to join the elements. (GL_TRIANGLES, GL_LINES, etc.) */
    GLuint _draw_mode;
    /** The vertex information */
    SP_Vertex_Buffer _vertex_buffer;
    /** The elements */
    SP_Element_Buffer _element_buffer;

};

typedef shared_ptr<Vertex_Array> SP_Vertex_Array;

#endif
