#include "gl_error.hpp"
#include "vertex_buffer.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

Vertex_Buffer::Vertex_Buffer()
    : _num_vertices(0),
      _name("NO NAME"),
      _static_access(true)
{
    _values = unique_ptr<float[]>(new GLfloat[1]);
    _handle = -1;
}

Vertex_Buffer::~Vertex_Buffer() {
    // cout << "VERTEX Buffer DESTRUCTOR! " << _name
    //      << " @" << this
    //      << " (handle " << _handle << ")" << endl;

    destroy_GPU_objects();
}

Vertex_Buffer::Vertex_Buffer(int num_vertices,
                             const vector<int>& attribute_dimensions,
                             const string& name,
                             bool static_access)
    : _attribute_dimensions(attribute_dimensions),
      _num_vertices(num_vertices),
      _name(name),
      _static_access(static_access)
{
    _total_dimension = get_total_dimension(attribute_dimensions);
    initialize(num_vertices);
    _handle = -1;
}

Vertex_Buffer::Vertex_Buffer(const vector<float>& values,
                             const vector<int>& attribute_dimensions,
                             const string& name,
                             bool static_access)
    : _attribute_dimensions(attribute_dimensions),
      _name(name),
      _static_access(static_access)
{
    _total_dimension = get_total_dimension(attribute_dimensions);
    _num_vertices = values.size() / _total_dimension;
    if (_num_vertices * _total_dimension != values.size()) {
        stringstream ss;
        ss << "\nVertex_Buffer constructor (name=" << name << "): ";
        ss << "#vertices * (total dimension) != values.size!" << endl;
        ss << "#vertices = " << _num_vertices;
        ss << " total dim = " << _total_dimension;
        ss << " size = " << values.size() << endl;
        throw runtime_error(ss.str());
    }
    initialize(_num_vertices);

    // Populate the values
    for (int i = 0; i < values.size(); i++)
        _values[i] = values[i];
    int total_dim = 0;
    for (int i = 0; i < _current_index.size(); i++) {
        _current_index[i] = total_dim;
        total_dim += attribute_dimensions[i];
    }
}

int Vertex_Buffer::get_total_dimension(const vector<int>& dimensions) {
    int total = 0;
    for (auto dimension : dimensions) {
        _current_index.push_back(total);
        total += dimension;
    }

    return total;
}

void Vertex_Buffer::initialize(int num_vertices) {
    int total_size = _total_dimension * num_vertices;
    _max_index = total_size - 1;
    total_size += 4; // safety margin to avoid buffer overflow (just in case)

    _values = unique_ptr<float[]>(new GLfloat[total_size]);
    //_values.reserve(total_size);
}

void Vertex_Buffer::destroy_GPU_objects() const {
    glDeleteBuffers(1, &_handle);
    GL_Error::check("Vertex_Buffer::destroy. DeleteBuffers failed");
}

void Vertex_Buffer::add(int attribute, const vec2& v) {
    if (_attribute_dimensions[attribute] != 2)
        GL_Error::die_or_continue(
            "called add(vec2) but attribute does not have dimension 2");
    add_values(attribute, v.x, v.y, 0, 1);
}

void Vertex_Buffer::add(int attribute, const vec3& v) {
    if (_attribute_dimensions[attribute] != 3)
        GL_Error::die_or_continue(
            "called add(vec3) but attribute does not have dimension 3");
    add_values(attribute, v.x, v.y, v.z, 1);
}

void Vertex_Buffer::add(int attribute, const vec4& v) {
    if (_attribute_dimensions[attribute] != 4)
        GL_Error::die_or_continue(
            "called add(vec4) but attribute does not have dimension 4");
    add_values(attribute, v.x, v.y, v.z, v.w);
}

int Vertex_Buffer::get_next_index(int attribute) {
    int index = _current_index[attribute];
    if (index > _max_index) {
        stringstream ss;
        ss << "\nVertex_Buffer::get_next_index(" << attribute << "):";
        ss << " Buffer overflow\n";
        ss << "index = " << index << " but max index = " << _max_index << endl;
        throw runtime_error(ss.str());
    }
    return index;
}

void Vertex_Buffer::add(int attribute,
                        float x, float y) {
    if (_attribute_dimensions[attribute] != 2)
        GL_Error::die_or_continue(
            "called add(x,y) but attribute does not have dimension 2");
    add_values(attribute, x, y, 0, 1);
}

void Vertex_Buffer::add(int attribute,
                        float x, float y, float z) {
    if (_attribute_dimensions[attribute] != 3)
        GL_Error::die_or_continue(
            "called add(x,y,z) but attribute does not have dimension 3");
    add_values(attribute, x, y, z, 1);
}

void Vertex_Buffer::add(int attribute,
                        float x, float y, float z, float w) {
    if (_attribute_dimensions[attribute] != 4)
        GL_Error::die_or_continue(
            "called add(x,y,z,w) but attribute does not have dimension 4");
    add_values(attribute, x, y, z, w);
}

void Vertex_Buffer::add_values(int attribute,
                               float x, float y, float z, float w) {
    int start_index = get_next_index(attribute);
    int index = start_index;
    _values[index++] = x;
    _values[index++] = y;
    int dimension = _attribute_dimensions[attribute];
    if (dimension > 2)
        _values[index++] = z;
    if (dimension > 3)
        _values[index++] = w;
    _current_index[attribute] = start_index + _total_dimension;
}

void Vertex_Buffer::install_data() {
    glGenBuffers(1, &_handle);
    GL_Error::check("Vertex_Buffer ctor. GenBuffers failed");

    glBindBuffer(GL_ARRAY_BUFFER, _handle);
    GL_Error::check("Vertex_Buffer::install_data. BindBuffer failed");

    GLuint access_mode = _static_access ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
    glBufferData(GL_ARRAY_BUFFER, (_max_index + 1) * sizeof(GLfloat),
                 &(_values[0]), access_mode);
    GL_Error::check("Vertex_Buffer::install_data. BufferData failed");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_Error::check("Vertex_Buffer::install_data. un-BindBuffer failed");
}

void Vertex_Buffer::define_attributes() const {
    glBindBuffer(GL_ARRAY_BUFFER, _handle);
    GL_Error::check("Vertex_Buffer::define_attributes. BindBuffer failed");

    // Tell the gpu how the vertex attributes are laid out.
    int start_index = 0;
    for (int attribute = 0; attribute < _attribute_dimensions.size();
         attribute++) {
        int dimension = _attribute_dimensions[attribute];

        glEnableVertexAttribArray(attribute);
        GL_Error::check(
          "Vertex_Buffer::define_attributes. glEnableVertexAttribArray failed");

         glVertexAttribPointer(attribute, dimension, GL_FLOAT, GL_FALSE,
                              _total_dimension * sizeof(GLfloat),
                              (void*)start_index);
        GL_Error::check(
            "Vertex_Buffer::define_attributes. Bad VertexAttribPointer");

        start_index += dimension * sizeof(GLfloat);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_Error::check("Vertex_Buffer::install_data. un-BindBuffer failed");
}

GLuint Vertex_Buffer::get_handle() const {
    return _handle;
}

string Vertex_Buffer::get_name() const {
    return _name;
}

int Vertex_Buffer::get_num_attributes() const {
    return _attribute_dimensions.size();
}

int Vertex_Buffer::get_num_vertices() const {
    return _num_vertices;
}


void Vertex_Buffer::update_values(int attribute, int vertex_index,
                                  float x, float y, float z, float w) {
    if (_static_access) {
        stringstream ss;
        ss << "\nVertex_Buffer::update (name=" << _name << "):" << endl;
        ss << "Can't update (static access only)" << endl;
        throw runtime_error(ss.str());
    }
    int start_index = vertex_index * _total_dimension;
    for (int i = 0; i < attribute; i++)
        start_index += _attribute_dimensions[i];
    int dimension = _attribute_dimensions[attribute];
    int index = start_index;

    // Update our CPU copy of the data (for debugging, mostly)
    _values[index++] = x;
    _values[index++] = y;
    if (dimension > 2)
        _values[index++] = z;
    if (dimension > 3)
        _values[index++] = w;

    // Update the data on the GPU's buffer (the real reason we called update)
    float values[] = {x, y, z, w};

    glBindBuffer(GL_ARRAY_BUFFER, _handle);
    GL_Error::check("Vertex_Buffer::update(): bind buffer");

    index = start_index;
    glBufferSubData(GL_ARRAY_BUFFER,
                    index * sizeof(GLfloat),
                    dimension * sizeof(GLfloat),
                    values);
    GL_Error::check("Vertex_Buffer::update(): buffer subdata");

    // ptr = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    // cout << "Mapped buffer AFTER update:" << endl;
    // for (int i=0; i < _num_vertices * _total_dimension; i++)
    //     cout << (int)ptr[i] << " ";
    // cout << endl;
    // // make sure to tell OpenGL we're done with the pointer
    // mapped_ok = glUnmapBuffer(GL_ARRAY_BUFFER);
    // cout << "Mapping succeeded? " << (mapped_ok ? "True" : "False") << endl;

    // cout << "After update, this is: " << *this << endl;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_Error::check("Vertex_Buffer::update(): un-bind buffer");
}

vec4 Vertex_Buffer::get(int attribute, int vertex_index) const {
    vec4 result(0,0,0,1);

    int start_index = vertex_index * _total_dimension;
    for (int i = 0; i < attribute; i++)
        start_index += _attribute_dimensions[i];
    int dimension = _attribute_dimensions[attribute];
    int index = start_index;

    result.x = _values[index++];
    result.y = _values[index++];
    if (dimension > 2)
        result.z = _values[index++];
    if (dimension > 3)
        result.w = _values[index++];
    return result;
}

void Vertex_Buffer::update(int attribute, int index, float x, float y) {
    if (_attribute_dimensions[attribute] != 2)
        GL_Error::die_or_continue(
            "called update(x,y) but attribute does not have dimension 2");
    update_values(attribute, index, x, y, 0, 1);
}

void Vertex_Buffer::update(int attribute, int index,
                           float x, float y, float z) {
    if (_attribute_dimensions[attribute] != 3)
        GL_Error::die_or_continue(
            "called update(x,y,z) but attribute does not have dimension 3");
    update_values(attribute, index, x, y, z, 1);
}

void Vertex_Buffer::update(int attribute, int index,
                           float x, float y, float z, float w) {
    if (_attribute_dimensions[attribute] != 4)
        GL_Error::die_or_continue(
            "called update(x,y,z,w) but attribute does not have dimension 4");
    update_values(attribute, index, x, y, z, w);
}

void Vertex_Buffer::update(int attribute, int index, const vec2& v) {
    if (_attribute_dimensions[attribute] != 2)
        GL_Error::die_or_continue(
            "called update(vec2) but attribute does not have dimension 2");
    update_values(attribute, index, v.x, v.y, 0, 1);
}

void Vertex_Buffer::update(int attribute, int index, const vec3& v) {
    if (_attribute_dimensions[attribute] != 3)
        GL_Error::die_or_continue(
            "called update(vec3) but attribute does not have dimension 3");
    update_values(attribute, index, v.x, v.y, v.z, 1);
}

void Vertex_Buffer::update(int attribute, int index, const vec4& v) {
    if (_attribute_dimensions[attribute] != 4)
        GL_Error::die_or_continue(
            "called update(vec4) but attribute does not have dimension 4");
    update_values(attribute, index, v.x, v.y, v.z, v.w);
}

vector<int> Vertex_Buffer:: get_attribute_dimensions() const {
    return _attribute_dimensions;
}

ostream& operator<<(ostream& os, const Vertex_Buffer& buf) {
    os << "Vertex_Buffer \"" << buf._name << "\""
       << " (handle " << buf.get_handle() << ")\n";
    int index = 0;
    for (int i = 0; i < buf._num_vertices; i++) {
        os << i << ": ";
        for (int i_dim = 0; i_dim < buf._attribute_dimensions.size(); i_dim++) {
            int dimension = buf._attribute_dimensions[i_dim];
            for (int k = 0; k < dimension; k++) {
                os << fixed << setw(6) << setprecision(3)
                   << buf._values[index++] << " ";
            }
            if (i_dim != buf._attribute_dimensions.size() - 1)
                os << "| ";
        }
        os << "\n";
    }
    return os;
}
