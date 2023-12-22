#include "image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::cout;
using std::endl;

Image::Image(const string& file_name) {
    // Load image, from file.
    // Tell stb_image.h to flip loaded texture on the y-axis.
    stbi_set_flip_vertically_on_load(true);
    _name = string("FILE " + file_name);
    unsigned char *data = stbi_load(file_name.c_str(),
                                    &_width, &_height, &_depth, 0);
    if (data == nullptr) {
        throw std::runtime_error(string("Image::ctor.  Can't load from file \"")
                                 + file_name + "\"");
    }
    _pixels = vector<unsigned char>();
    int num_pixels = _width * _height * _depth;
    _pixels.reserve(num_pixels);
    std::copy(data, data + num_pixels, _pixels.begin());
}

Image::Image(GLFWwindow *window) {
    glfwGetFramebufferSize(window, &_width, &_height);
    _depth = 3;
    _pixels = vector<unsigned char>();
    int num_pixels = _width * _height * _depth;
    _pixels.reserve(num_pixels);
    glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE,
                 _pixels.data());
}

Image::Image(const vector<unsigned char>& pixels,
             int width, int height, int depth,
             const string& name)
    : _pixels(pixels), _width(width), _height(height), _depth(depth),
      _name(name) {
    ;

    // std::cout << "Image ctor (" << width << " x " << _height << ")"
    //           << std::endl;
    // std::cout << "Image ctor (from bytes).  pixels=" << (long)pixels
    //           << std::endl;
}

Image::~Image() {
    ; // nothing to do.  _pixels will auto-delete its buffer
}

void Image::write_pnm(const string& file_name) {
    std::ofstream file;
    file.open(file_name, std::ofstream::binary);

    // std::cout << "Image::write_pnm(" << file_name << ")"
    //           << " depth=" << _depth << std::endl;

    std::stringstream header_stream;
    int target_bytes_per_pixel;
    if (_depth == 1) {
        header_stream << "P5\n";
        target_bytes_per_pixel = 1;
    }
    else {
        header_stream << "P6\n";
        target_bytes_per_pixel = 3;
    }

    header_stream << _width << " " << _height << "\n";
    header_stream << "255\n";
    string header = header_stream.str();
    file.write(header.c_str(), header.length());

    int row_width = _width * target_bytes_per_pixel;
    char *target_row = new char[row_width];

    // std::cout << "Image.write_pnm.  pixels=" << (long)_pixels << std::endl;
    // std::cout << " target_row at " << (long)target_row << std::endl;
    // std::cout << " row width=" << row_width << std::endl;

    for (int y = 0; y < _height; y++) {
        // Use (_height - y) because OpenGL has increasing y going up,
        // but pgm/ppm and most other file formats have
        // increasing y going down.
        int i_source = (_height - 1 - y) * _width * _depth;
        int i_target = 0;
        for (int x = 0; x < _width; x++) {
            // Red (or monochrome gray) pixel
            target_row[i_target++] = _pixels[i_source++];
            if (_depth > 1) {
                // Green and blue
                target_row[i_target++] = _pixels[i_source++];
                target_row[i_target++] = _pixels[i_source++];
            }
            if (_depth > 3) // skip alpha byte
                i_source++;
        }

        file.write(target_row, row_width);
    }

    file.close();
    delete[] target_row;
}

const vector<unsigned char>& Image::get_pixels() const {
    return _pixels;
}

int Image::get_width() const {
    return _width;
}

int Image::get_height() const {
    return _height;
}

int Image::get_depth() const {
    return _depth;
}

string Image::get_name() const {
    return _name;
}

float Image::get_aspect_ratio() const {
    return (float)_width / (float)_height;
}

ostream& operator<<(ostream& os, const Image& img) {
    os << "Image \"" << img._name << "\" (W x H) = "
       << "(" << img._width << " x " << img._height << ")"
       << " depth: " << img._depth << std::endl;
    return os;
}
