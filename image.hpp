#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using std::shared_ptr;
using std::string;
using std::ostream;
using std::vector;

class Image {
 public:
    /** Construct from a file.
     * @param file_name Name of the image file.
     */
    Image(const string& file_name);

    /** Construct from pixels.
     * @param pixels The pixels, a one-dimensional array.
     * @param width Width of the image.
     * @param height Height of the image.
     * @param depth Number of values per pixel.
     * @param name Name of the image (for debugging)
     */
    Image(const vector<unsigned char>& pixels, int width, int height, int depth,
          const string& name);

    /** Construct from pixels on the default framebuffer.
     * @param window The window whose pixels we'll read.
     */
    Image(GLFWwindow *window);

    /** Destructor.
     */
    ~Image();

    /** Write pixels to a .ppm or .pgm file.
     * @param file_name The file to write into.
     */
    void write_pnm(const string& file_name);

    /** Access the pixels.
     * @return The pixels.
     */
    const vector<unsigned char>& get_pixels() const;

    /** Access the width.
     * @return The width.
     */
    int get_width() const;

    /** Access the height.
     * @return The height.
     */
    int get_height() const;

    /** Access the depth.
     * @return The depth.
     */
    int get_depth() const;

    /** Access the name.
     * @return The name.
     */
    string get_name() const;

    /** Compute the aspect ratio (width / height)
     * @return The aspect ratio.
     */
    float get_aspect_ratio() const;

    /** Print out an image's metadata data neatly.
     * @param os the output stream.
     * @param img a Image.
     * @return the stream, after output.
     */
    friend ostream& operator<<(ostream& os, const Image& img);

 private:
    /** The pixels */
    vector<unsigned char> _pixels;
    /** The width */
    int _width;
    /** The height */
    int _height;
    /** Number of channels per pixel */
    int _depth;
    /** Name of source file */
    string _name;
};

typedef shared_ptr<Image> SP_Image;


#endif
