#version 330 core
// Pixel color is just a texel.
// In other words, paints the texture onto the mesh,
// with no lighting or other effects.

out vec4 FragColor;

in vec2 transformed_texture_coord;

uniform sampler2D current_texture;

void main()
{
    FragColor = texture(current_texture, transformed_texture_coord);
}
