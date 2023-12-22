#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include "vertex_array.hpp"
#include "program.hpp"

using namespace std;

/** A collection of Vertex_Arrays, all drawn with the same Shader_Program.
 *
 * It groups several parts of a polyhedral mesh.  Each part can be connected
 * differently.  For example a cylinder can consist of three parts:
 *
 * - a triangle strip for the curved sides,
 *
 * - a triangle fan for the flat circular top, and
 *
 * - a triangle fan for the flat circular bottom.
 */
class Mesh {
public:
    /** Constructor.
     *
     * @param program The Shader_Program to be used to draw all the parts.
     * @param name Name, used for debugging.
     * @param part Optional pointer to the first mesh part.  If omitted, the
     *             Mesh is constructed with zero parts, initially.
     */
    Mesh(SP_Shader_Program program,
         const string& name="NO NAME",
         SP_Vertex_Array part=SP_Vertex_Array());

    /** Destructor
     */
    ~Mesh();

    /** Add a part to the mesh.
     *
     * @param part Pointer to the part.
     */
    void add(SP_Vertex_Array part);

    /** Draw the mesh.
     */
    void draw() const;

    /** De-allocate the GPU structures for all the parts.  Calls destroy()
     * for each part.
     */
    void destroy() const;

    /** Get one attribute for one vertex in one part.
     *
     * This is just a wrapper around Vertex_Array::get().
     *
     * @param part The part index.
     * @param attribute The attribute index.
     * @param index The vertex index.
     * @return 4 values, in a vec4.
     */
    glm::vec4 get(int part, int attribute, int index) const;

    /** Retrieve the shader program.
     *
     * @return The shader program.
     */
    SP_Shader_Program get_program() const;

    /** Returns how many parts we have.
     *
     * @return How many parts.
     */
    int num_parts() const;

    /** Retrieve one part.
     *
     * @param i index of the part
     * @return Pointer to the part.
     */
    SP_Vertex_Array get_part(int i) const;

    /** Retrieve the name
     *
     * @return the name
     */
    string get_name() const;

    /** Output to stream (for debugging)
     */
    friend ostream& operator<<(ostream& os, const Mesh& mesh);

 private:
    SP_Shader_Program _program;
    vector<SP_Vertex_Array> _parts;
    string _name;

};

typedef shared_ptr<Mesh> SP_Mesh;

#endif
