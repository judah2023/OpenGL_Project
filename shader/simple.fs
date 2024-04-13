#version 330 core

in vec4 vertColor;
in vec2 texturePos;

out vec4 fragColor;

uniform float u_time;
uniform sampler2D tex;
uniform sampler2D tex_face;

void main()
{
    float sinTime = (sin(u_time) + 1) * 0.5;
    float cosTime = (cos(u_time) + 1) * 0.5;
    float sinTime2 = sinTime * sinTime;
    fragColor = texture(tex, texturePos) * 0.8 + texture(tex_face, texturePos) * 0.2;
}