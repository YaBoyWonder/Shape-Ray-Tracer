#ifndef ELEMENT_BUFFER_H
#define ELEMENT_BUFFER_H

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <glad/glad.h>

using namespace std;

/**-------------------------------------------------------------------
This class implements an Element Buffer, which stores a sequence
of indexes.  These indexes will be used with a Vertex Buffer,
telling the gpu which vertices should be connected together.

Example: We want to connect together vertices 3, 4, and 2:
```
SP_Element_Buffer eb(new Element_Buffer(3, "My Triangle Indexes"));
eb->add(3);
eb->add(4);
eb->add(2);

// Another way to construct it:
SP_Element_Buffer eb(new Element_Buffer(vector<int>{3, 4, 2},
                                        "My Triangle Indexes"));

// Copy data onto gpu
eb->install_data();

// Print out contents
cout << *eb.get() << endl;
```
*/

class Element_Buffer {
private:
    unique_ptr<GLuint[]> _values;
    //GLuint* _values;
    //vector<GLuint> _values;
    GLuint _handle;
    int _current_index;
    int _size;
    string _name;
    bool _static_access;

public:
    /** Default constructor
     */
    Element_Buffer();

    /** Destructor
     */
    ~Element_Buffer();

    /** Parameterized constructor.
     *
     * @param num_indexes How many indexes to allocate for
     * @param name Name (for debugging)
     * @param static_access Hint to ibrary whether we'll modify values.
     */
    Element_Buffer(int num_indexes,
                   const string& name="NO NAME",
                   bool static_access=true);

    /** Parameterized constructor.
     *
     * @param values values to intialize the buffer.
     * @param name Name (for debugging)
     * @param static_access Hint to ibrary whether we'll modify values.
     */
    Element_Buffer(const vector<int> values,
                   const string& name="NO NAME",
                   bool static_access=true);

    /** Add the next index to the buffer.
     *
     * @param index The index
     */
    void add(int index);

    /** Replace an index in the buffer.
     *
     * @param location The index's location in the buffer
     * @param new_index The index's new value
     */
    void set(int location, int new_index);

    /** Retrieve an index from the buffer.
     *
     * @param location The index's location in the buffer
     */
    GLuint get(int location) const;

    /** Copy the buffer's data onto the GPU's buffer.
     *
     * Hides these OpenGL calls from the client:
```
glBindBuffer : activate the element buffer
glBufferData : copy data onto the GPU's element buffer object.
```
    */
    void install_data();

    /** Delete the element buffer on the GPU.
     */
    void destroy_GPU_objects() const;

    /** Retrieve the element buffer object (an int handle)
     */
    GLuint get_handle() const;

    /** Retrieve the name.
     */
    string get_name() const;

    /** Retrieve how many indexes are stored.
     */
    int get_size() const;

    /** Print out the element buffer's data neatly.
     * @param os the output stream
     * @param buf an Element_Buffer
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Element_Buffer& buf);
};

typedef shared_ptr<Element_Buffer> SP_Element_Buffer;

#endif
