#include "mesh.hpp"

using namespace std;

Mesh::Mesh(SP_Shader_Program program,
           const string& name, SP_Vertex_Array first_part)
    : _program(program),
      _name(name)
{
    if (first_part.get() != nullptr)
        add(first_part);
}

void Mesh::add(SP_Vertex_Array part) {
    _parts.push_back(part);
}

void Mesh::draw() const {
    for (auto part : _parts) {

        // cout << "Mesh::draw().  name = " << _name
        //      << ". program handle = " << _program->get_handle() << endl;

        // cout << "Gonna draw mesh part @" << part << endl;
        // cout << " part is " << *part << endl;

        // cout << "Program is " << *_program << endl;

        part->draw(_program);
    }
}

Mesh::~Mesh() {
    // cout << "Mesh DESTRUCTOR" << endl;

    destroy();
}

void Mesh::destroy() const {
    for (auto part : _parts)
        part->destroy();
}

glm::vec4 Mesh::get(int part, int attribute, int index) const {
    return _parts[part]->get(attribute, index);
}

SP_Shader_Program Mesh::get_program() const {
    return _program;
}

int Mesh::num_parts() const {
    return _parts.size();
}

SP_Vertex_Array Mesh::get_part(int i) const {
    return _parts[i];
}

string Mesh::get_name() const {
    return _name;
}

ostream& operator<<(ostream& os, const Mesh& mesh) {
    os << "Mesh \"" << mesh._name << "\""
       << " #parts:    " << mesh._parts.size() << endl;
    for (int i = 0; i < mesh._parts.size(); i++) {
        cout << "  part #" << i << ":" << endl;
        cout << *mesh._parts[i] << endl;
    }
    return os;
}
