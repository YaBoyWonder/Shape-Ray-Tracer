#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;

/**
 * A collection of vertices, with attributes.
 * This is a Vertex Buffer object, in OpenGL-speak.
 *
 * It keeps an array of floats here on the CPU's RAM,
 * which get copied to the GPU by calling the install_data() method.
 *
 * Example: a 3D triangle, with a normal vector at each vertex:
 *   Each vertex has 3 floats for the vertex position,
 *   and 3 floats for the normal:
```
// 3 floats for position, 3 for normal
std::vector<int> vertex_dimensions{3, 3};
int num_vertices = 3;
Vertex_Buffer *vb = new Vertex_Buffer(num_vertices, vertex_dimensions,
                                      "My Triangle");
// The two vertex attributes;
const int position = 0;  // position is first, at index 0
const int normal = 1;    // normal is second, at inde 1
// the 3 vertex positions:
vb.add(position, 0, 0, 0);
vb.add(position, 1, 0, 0);
vb.add(position, 0, 1, 0);

// the 3 normals (all pointing along z axis)
vb.add(normal, 0, 0, 1);
vb.add(normal, 0, 0, 1);
vb.add(normal, 0, 0, 1);

// Here is another way to construct it, with the same data:
SP_Vertex_Buffer vb(new Vertex_Buffer(vector<float>{0,0,0, 0,0,1,
                                                    1,0,0, 0,0,1,
                                                    0,1,0, 0,0,1},
                                      vertex_dimensions, "My Triangle"));

// Load the data onto the gpu
vb->install_data();

// Print out the contents
cout << *vb.get() << endl;
```
*/

class Vertex_Buffer {
 public:
    /** Default Constructor */
    Vertex_Buffer();
    /** Destructor.
     * Deletes array of values, and calls destroy_GPU_objects()
     */
    ~Vertex_Buffer();

    /** Parameterized constructor.
     * @param num_vertices How many vertices to allocate for.
     * @param attribute_dimensions How many floats each attribute takes up.
     * @param name Name or the buffer, for debugging.
     * @param static_access Whether values may be changed later by the program,
     *  hint to driver as to where GPU values should reside.
     */
    Vertex_Buffer(int num_vertices,
                  const vector<int>& attribute_dimensions,
                  const string& name="NO NAME",
                  bool static_access=true);
    /** Parameterized constructor.
     * @param values Initialize the array with these floats.
     * @param attribute_dimensions How many floats each attribute takes up.
     * @param name Name or the buffer, for debugging.
     * @param static_access Whether values may be changed later by the program,
     *  hint to driver as to where GPU values should reside.
     */
    Vertex_Buffer(const vector<float>& values,
                  const vector<int>& attribute_dimensions,
                  const string& name="NO NAME",
                  bool static_access=true);

    /** Add one more vertex's values to the array.
     *
     * Each attribute may have 1, 2, 3, or 4 values, depending on the
     * attribute's dimension.  This method will only use all 4 parameters
     * if the dimension is 4.  Otherwise some parameters (y, z, or w)
     * will be ignored.
     * @param attribute Which vertex attribute these values refer to.
     * @param x First value.
     * @param y Second value.
     * @param z Third value.
     * @param w Fourth value.
     */
    void add_values(int attribute, float x, float y, float z, float w);

    /** Add one more vertex's values to the array.
     *
     * Dimension must be 2, otherwise will complain and maybe terminate.
     * @param attribute Which vertex attribute these values refer to.
     * @param x First value.
     * @param y Second value.
     */
    void add(int attribute, float x, float y);

    /** Add one more vertex's values to the array.
     *
     * Dimension must be 3, otherwise will complain and maybe terminate.
     * @param attribute Which vertex attribute these values refer to.
     * @param x First value.
     * @param y Second value.
     * @param z Third value.
     */
    void add(int attribute, float x, float y, float z);

    /** Add one more vertex's values to the array.
     *
     * Dimension must be 4, otherwise will complain and maybe terminate.
     * @param attribute Which vertex attribute these values refer to.
     * @param x First value.
     * @param y Second value.
     * @param z Third value.
     * @param w Fourth value.
     */
    void add(int attribute, float x, float y, float z, float w);

    /** Add one more vertex's values to the array.
     *
     * Dimension must be 2, otherwise will complain and maybe terminate.
     * @param attribute Which vertex attribute these values refer to.
     * @param v Two floats.
     */
    void add(int attribute, const vec2& v);

    /** Add one more vertex's values to the array.
     *
     * Dimension must be 3, otherwise will complain and maybe terminate.
     * @param attribute Which vertex attribute these values refer to.
     * @param v Three floats.
     */
    void add(int attribute, const vec3& v);

    /** Add one more vertex's values to the array.
     *
     * Dimension must be 4, otherwise will complain and maybe terminate.
     * @param attribute Which vertex attribute these values refer to.
     * @param v Four floats.
     */
    void add(int attribute, const vec4& v);

    /** Retrieve a vertex's values from the array.
     *
     * Always returns 4 values, regardless of the attribute's dimension.
     * If dimension < 4, sets w to 1.
     * If dimension < 3, sets z to 0.
     * @param attribute Which vertex attribute these values refer to.
     * @param vertex_index Which vertex.
     * @return 4 floats in a vec4
     */
    vec4 get(int attribute, int vertex_index) const;

    /** Change one attribute for one vertex.
     *
     * All the update() methods call this method.
     *
     * Changes the array on the CPU side and also the buffer on the GPU.
     * It hides the following openGL functions from the client:
```
glBindBuffer    : activate a buffer on the GPU.
glBufferSubData : copy floats from the CPU array to the GPU.
```
     * @param attribute Which attribute
     * @param index Which vertex
     * @param x First value.
     * @param y Second value.
     * @param z Third value.
     * @param w Fourth value.
     */
    void update_values(int attribute, int index,
                       float x, float y, float z, float w);

    /** Change one attribute for one vertex.
     *
     * Dimension must be 2, otherwise will complain and maybe terminate.
     * @param attribute Which attribute
     * @param index Which vertex
     * @param x First value.
     * @param y Second value.
     */
    void update(int attribute, int index, float x, float y);

    /** Change one attribute for one vertex.
     *
     * Dimension must be 3, otherwise will complain and maybe terminate.
     * @param attribute Which attribute
     * @param index Which vertex
     * @param x First value.
     * @param y Second value.
     * @param z Third value.
     */
    void update(int attribute, int index, float x, float y, float z);

    /** Change one attribute for one vertex.
     *
     * Dimension must be 4, otherwise will complain and maybe terminate.
     * @param attribute Which attribute
     * @param index Which vertex
     * @param x First value.
     * @param y Second value.
     * @param z Third value.
     * @param w Fourth value.
     */
    void update(int attribute, int index, float x, float y, float z, float w);

    /** Change one attribute for one vertex.
     *
     * Dimension must be 2, otherwise will complain and maybe terminate.
     * @param attribute Which attribute
     * @param index Which vertex
     * @param v The new values.
     */
    void update(int attribute, int index, const vec2& v);

    /** Change one attribute for one vertex.
     *
     * Dimension must be 3, otherwise will complain and maybe terminate.
     * @param attribute Which attribute
     * @param index Which vertex
     * @param v The new values.
     */
    void update(int attribute, int index, const vec3& v);

    /** Change one attribute for one vertex.
     *
     * Dimension must be 4, otherwise will complain and maybe terminate.
     * @param attribute Which attribute
     * @param index Which vertex
     * @param v The new values.
     */
    void update(int attribute, int index, const vec4& v);

    /** Copy the CPU array's values to the GPU's vertex buffer.
     *
     * Hides these openGL functions from the client:
```
glGenBuffers : create a vertex buffer object on the GPU
glBindBuffer : activate the vertex buffer
glBufferData : copy from CPU to GPU
```
    */
    void install_data();

    /** Tell the GPU about the layout of the values in the CPU array.
     *
     * Hides these openGL functions from the client:
```
glBindBuffer              : activate the vertex buffer
glEnableVertexAttribArray : activate one attribute on the GPU buffer
glVertexAttribPointer     : tell the GPU where to find the floats for one of the vertex attributes.
```
    */
    void define_attributes() const;

    /** Deallocate the GPU's vertex buffer.
     */
    void destroy_GPU_objects() const;

    /** Access the vertex buffer "object" (an int handle)
     * @return the handle
     */
    GLuint get_handle() const;

    /** Access the vertex buffer's name (for debugging output).
     * @return the name
     */
    string get_name() const;

    /** Returns number of attributes.
     * @return how many attributes each vertex has
     */
    int get_num_attributes() const;

    /** Returns number of vertices.
     * @return How many vertices
     */
    int get_num_vertices() const;

    /** Returns, well, the dimensions of the vertex attributes.
     * @return the attribute dimensions.
     */
    vector<int> get_attribute_dimensions() const;

    /** Print out the vertex buffer's data neatly.
     * @param os the output stream
     * @param buf a Vertex_Buffer
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Vertex_Buffer& buf);

 private:
    /** The array that holds the vertex attributes, on the CPU side */

    //GLfloat* _values;
    unique_ptr<GLfloat[]> _values;
    //vector<GLfloat> _values;
    /** The Vertex Buffer object (an int handle) */
    GLuint _handle;
    /** The current indexes, for all the attributes */
    vector<int> _current_index;
    /** How many float each attribute needs */
    vector<int> _attribute_dimensions;
    /** How many vertices */
    int _num_vertices;
    /** Maximum index, for bounds checking */
    int _max_index;
    /** How many floats, in total, per vertex */
    int _total_dimension;
    /** Name, for debugging */
    string _name;
    /** Should the GPU allow us to change the contents */
    bool _static_access;

    /** Array index where data should go, for each attribute */
    int get_next_index(int attribute);
    /** Compute _total_dimension */
    int get_total_dimension(const vector<int>& attribute_dimensions);
    /** Set up the array, the indexes and dimension info */
    void initialize(int num_vertices);

};

typedef shared_ptr<Vertex_Buffer> SP_Vertex_Buffer;

#endif
