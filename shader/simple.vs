#version 330 core
layout (location = 0) in vec3 aPos;

uniform float u_time;

void main()
{
    float sinTime = sin(u_time);
    float cosTime = cos(u_time);

    vec3 newPos = vec3(cosTime * aPos.x - sinTime * aPos.y, 
                       sinTime * aPos.x + cosTime * aPos.y, 
                       aPos.z);

    gl_Position = vec4(newPos, 1.0);
}