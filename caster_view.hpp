#ifndef _CASTER_VIEW_HPP
#define _CASTER_VIEW_HPP

#include "image.hpp"
#include "mesh.hpp"
#include "texture.hpp"

class Caster_View {
 public:
    /** Default constructor.
     */
    Caster_View();

    /** Paint an image onto the screen, filling it.
     */
    void draw(SP_Image image);

 private:
    SP_Mesh _square_mesh;
    SP_Texture _texture;
};

#endif
