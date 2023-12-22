#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <glad/glad.h>

#include "image.hpp"

using namespace std;

class Texture {
 public:
    /** Initialize the texture, using data from an image.
     * It hides the following openGL functions from the client:
```
glGenTextures    : create texture object
glBindTexture    : make current texture active
glTexParameteri  : set various texture-mapping parameters:
                   coordinate wrapping, and
                   maginification and minification filters.
glTexImage2D     : copy texture pixels to the GPU
glGenerateMipmap : generate MIPmaps for minification
```
     * @param image The source image
     */
    Texture(SP_Image image);

    /** Initialize Texture from a given GPU texture object.
     * @param handle The texture object.
     * @param width width in pixels
     * @param height height in pixels
     * @param pixel_format format
     * @param num_channels #values per pixel.
     * @param name Name to give the texture.
     */
    Texture(GLuint handle,
            int width,
            int height,
            GLuint pixel_format,
            int num_channels,
            const string& name);

    /** Deallocate the texture on the GPU.
     */
    ~Texture();

    /** Attach the texture to a texture unit.
     * It hides the following openGL functions from the client:
```
glActiveTexture : activate a texture unit.
glBindTexture   : attach texture to currently-active texture unit
```
     * @param unit Which texture unit to attach to.
     */
    void attach_texture_unit(GLuint unit) const;

    /** Get texture's handle (texture object on the GPU).
     * @ return The handle.
     */
    GLuint get_handle() const;

    /** Fetch pixels from the GPU, into an Image object.
     * @return The Image.
     */
    SP_Image read_pixels() const;

    /** Print out the texture's metadata data neatly.
     * @param os the output stream
     * @param tex a Texture
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Texture& tex);

 private:
    /** The Texture object (an int handle) */
    GLuint _handle;
    /** Pixel format (GL_RGB or GL_RGBA) */
    GLuint _pixel_format;
    /** The width */
    int _width;
    /** The height */
    int _height;
    /** Number of channels per pixel */
    int _depth;
    /** Name of texture */
    string _name;
};

typedef shared_ptr<Texture> SP_Texture;

#endif
