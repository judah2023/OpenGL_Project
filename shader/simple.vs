#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexture;

out vec4 vertColor;
out vec2 texturePos;

uniform mat4 transMat;

void main()
{
    gl_Position = transMat * vec4(vPos, 1.0);
    vertColor = vec4(vColor, 1.0);
    texturePos = vTexture;
}