#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;

out vec4 vertColor;
out vec2 texturePos;

uniform float u_time;

mat3 rotate2D(float _angle)
{
    float sinTime = sin(u_time);
    float cosTime = cos(u_time);

    return mat3(cosTime, -sinTime, 0.0,
                sinTime, cosTime, 0.0,
                0.0, 0.0, 1.0);
}

void main()
{
    vec3 newPos = rotate2D(u_time) * vPos;
    gl_Position = vec4(newPos, 1.0);
    vertColor = vec4(vColor, 1.0);
    texturePos = vTexture;
}