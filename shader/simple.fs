#version 330 core

in vec4 vertColor;

out vec4 fragColor;

uniform float u_time;

void main()
{
    float sinTime = (sin(u_time) + 1) * 0.5;
    float cosTime = (cos(u_time) + 1) * 0.5;
    float sinTime2 = sinTime * sinTime;
    fragColor = vec4(sinTime, cosTime, sinTime2, 1.0f) * vertColor;
    //fragColor = vec4(vertColor.x * sinTime, vertColor.y * cosTime, vertColor.z * sinTime2, vertColor.w);
}