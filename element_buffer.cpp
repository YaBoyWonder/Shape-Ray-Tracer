#include <stdexcept>
#include <iomanip>
#include "gl_error.hpp"
#include "element_buffer.hpp"

Element_Buffer::Element_Buffer()
    : _size(0),
      _name("NO NAME"),
      _static_access(true)
{
    _values = unique_ptr<GLuint[]>(new GLuint[1]);
    _handle = -1;
}

Element_Buffer::Element_Buffer(int num_indexes,
                               const string& name,
                               bool static_access)
    : _size(num_indexes),
      _name(name),
      _static_access(static_access)
{
    glGenBuffers(1, &_handle);
    GL_Error::check("Element_Buffer ctor. GenBuffers failed");

    _current_index = 0;
    _values = unique_ptr<GLuint[]>(new GLuint[_size]);
    // _values.reserve(_size);
}


Element_Buffer::~Element_Buffer() {
    // cout << "Element Buffer DESTRUCTOR! " << _name
    //      << " @" << this
    //      << " (handle " << _handle << ")" << endl;

    destroy_GPU_objects();
}

Element_Buffer::Element_Buffer(const vector<int> values,
                               const string& name,
                               bool static_access)
    : _size(values.size()),
      _name(name),
      _static_access(static_access)
{
    _current_index = 0;
    _values = unique_ptr<GLuint[]>(new GLuint[_size]);
    // _values.reserve(_size);

    for (int i = 0; i < _size; i++)
        _values[i] = values[i];
    _current_index = _size;
    _handle = -1;
}

void Element_Buffer::destroy_GPU_objects() const {
    glDeleteBuffers(1, &_handle);
    GL_Error::check("Element_Buffer destructor. DeleteBuffers failed");
}

void Element_Buffer::add(int index) {
    if (_current_index > _size - 1)
        throw runtime_error("Buffer overflow");
    _values[_current_index++] = index;

}

void Element_Buffer::set(int index, int value) {
    if (_static_access)
        throw runtime_error("Can't update element buffer (static access only)");
    if (index < 0 || index > _size - 1)
        throw runtime_error("Index out of bounds");
    _values[index] = value;

}

void Element_Buffer::install_data() {
    glGenBuffers(1, &_handle);
    GL_Error::check("Element_Buffer ctor. GenBuffers failed");

    glBindBuffer(GL_ARRAY_BUFFER, _handle);
    GL_Error::check("Element_Buffer::install_data. BindBuffer failed");

    GLuint access_mode = GL_STATIC_DRAW;
    if (!_static_access)
        access_mode = GL_DYNAMIC_DRAW;
    glBufferData(GL_ARRAY_BUFFER, _size * sizeof(GLuint),
                 &(_values[0]), access_mode);
    // glBufferData(GL_ARRAY_BUFFER, _size * sizeof(GLuint),
    //              _values.data(), access_mode);
    GL_Error::check("Element_Buffer::install_data. BufferData failed");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GL_Error::check("Element_Buffer::install_data. un-BindBuffer failed");

    // cout << "Element Buffer after installing:" << endl;
    // cout << *this << endl;
}


GLuint Element_Buffer::get_handle() const {
    return _handle;
}

string Element_Buffer::get_name() const {
    return _name;
}

int Element_Buffer::get_size() const {
    return _size;
}


GLuint Element_Buffer::get(int index) const {
    return _values[index];
}

ostream& operator<<(ostream& os, const Element_Buffer& buf) {
    os << "Element_Buffer \"" << buf._name
       << "\" (handle " << buf.get_handle() << ")\n";
    int values_per_line = 10;
    for (int chunk_start = 0; chunk_start < buf._size;
         chunk_start += values_per_line) {
        os << "  " << setw(3) << chunk_start << " to " << setw(3)
           << min(chunk_start + values_per_line - 1, buf._size - 1) << " : ";
        for (int i = chunk_start;
             i < min(chunk_start + values_per_line, buf._size);
             i++)
            os << setw(2) << buf._values[i] << " ";
        os << "\n";
    }
    return os;
}
