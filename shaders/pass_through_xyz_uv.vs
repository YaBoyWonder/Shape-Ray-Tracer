//////////////////////////
// A vertex shader that passes position and texture coords to fragment shader
//
// Each vertex has a position and texture coords.
// No transformations are applied (assume we're drawing into
//  NDCS coords, where x, y, z all range from -1 to +1).
//
//////////////////////////

#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texture_coord;

out vec2 transformed_texture_coord;

void main()
{
    gl_Position = position;
    transformed_texture_coord = texture_coord;
}

