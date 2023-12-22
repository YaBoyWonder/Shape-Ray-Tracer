#include "texture.hpp"
#include "gl_error.hpp"

Texture::Texture(GLuint handle,
                 int width,
                 int height,
                 GLuint pixel_format,
                 int depth,
                 const string& name)
    : _handle(handle),
      _pixel_format(pixel_format),
      _width(width), _height(height),
      _depth(depth),
      _name(name) {
    ;

    // cout << "Texture ctor.  _handle=" << handle << " name=" << name << endl;

}

Texture::Texture(SP_Image image) {
    glGenTextures(1, &_handle);
    GL_Error::check("Texture::ctor. glGenTextures failed");

    glBindTexture(GL_TEXTURE_2D, _handle);
    GL_Error::check("Texture::ctor. glBinTextures failed");

    // Set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GL_Error::check("Texture::ctor. glTexParameteri failed");

    _width  = image->get_width();
    _height = image->get_height();
    _depth  = image->get_depth();
    _name   = image->get_name() + " texture";
    const vector<unsigned char>& pixels = image->get_pixels();

    //cout << "Texture ctor.  _handle=" << _handle << " name=" << _name << endl;

    // Make an image with 4 bytes per pixel,
    // even if file's image only has 3/pixel.
    // (GPUs like to align pixels on 4-byte boundaries).
    vector<char> texels(_width * _height * 4);
    int i_src = 0;
    int i_dst = 0;
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            texels[i_dst++] = pixels[i_src++];
            texels[i_dst++] = pixels[i_src++];
            texels[i_dst++] = pixels[i_src++];
            if (_depth == 4) {
                // PNG transparency pixel (OpenGL calls this "alpha")
                texels[i_dst++] = pixels[i_src++];
            }
            else {
                // No transparency, pixel is fully opaque (byte=255)
                texels[i_dst++] = '\xff';
            }
        }
    }

    _pixel_format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, _pixel_format,
                 _width, _height, 0, _pixel_format,
                 GL_UNSIGNED_BYTE, texels.data());
    GL_Error::check("Texture::ctor. glTexImage2D failed");

    glGenerateMipmap(GL_TEXTURE_2D);
    GL_Error::check("Texture::ctor. glGenerateMipmap failed");
}

Texture::~Texture() {
    glDeleteTextures(1, &_handle);
}

void Texture::attach_texture_unit(GLuint unit) const {

    // cout << "Texture::attach(" << unit << "): handle=" << _handle << endl;

    glBindTexture(GL_TEXTURE_2D, _handle);
    GL_Error::check("Texture::attach_texture_unit. glBindTexture failed");

    glActiveTexture(GL_TEXTURE0 + unit);
    GL_Error::check("Texture::attach_texture_unit. glActiveTexture failed");
}

GLuint Texture::get_handle() const {
    return _handle;
}

SP_Image Texture::read_pixels() const {
    int source_bytes_per_pixel = 1;

    int num_bytes = 0;
    switch (_pixel_format) {
    case GL_DEPTH_COMPONENT:
        source_bytes_per_pixel = 1;
        break;
    case GL_RGB:
        source_bytes_per_pixel = 3;
        break;
    case GL_RGBA:
        source_bytes_per_pixel = 4;
        break;
    default:
        GL_Error::die_or_continue(
            "Can't save texture that isn't depth, rgb, or rbga");
    }

    num_bytes = _width * _height * source_bytes_per_pixel;

    // Allocate buffer to receive texture, from GPU.
    vector<unsigned char> pixels;
    pixels.reserve(num_bytes);

    // cout << "Texture::read_pixels.  pixels @" << long(pixels) << endl;
    // cout << "Texture::read_pixels. depth=" << _depth
    //      << " num_pixels=" << num_bytes << endl;

    // Transfer from GPU into buffer
    glBindTexture(GL_TEXTURE_2D, _handle);
    GL_Error::check("Texture::read_pixels. glBindTexture failed");

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    GL_Error::check("Texture::read_pixels. glPixelStorei failed");

    glGetTexImage(GL_TEXTURE_2D, 0, _pixel_format, GL_UNSIGNED_BYTE,
                  pixels.data());
    GL_Error::check("Texture::read_pixels. glGetTexImage failed");

    // cout << "Texture::read_pixels. \"" << _name << "\" "
    //      << "(" << _width << " x " << _height << ")"
    //      << " depth=" << _depth << " #bytes=" << num_bytes << endl;

    return SP_Image(
        new Image(pixels, _width, _height, _depth, _name + " image"));
}

ostream& operator<<(ostream& os, const Texture& tex) {
    os << "Texture \"" << tex._name << "\""
       << " handle " << tex._handle
       << " (" << tex._width << " x " << tex._height << ", "
       << tex._depth << " values/pixel)" << std::endl;
    return os;
}
