#version 330 core

in vec4 vertColor;
in vec2 texturePos;

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D tex_face;

void main()
{
    fragColor = texture(tex, texturePos) * 0.8 + texture(tex_face, texturePos) * 0.2;
}