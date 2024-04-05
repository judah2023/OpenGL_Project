#version 330 core
out vec4 fragColor;

uniform float u_time;

void main()
{
    float sinTime = (sin(u_time) + 1) * 0.5;
    float cosTime = (cos(u_time) + 1) * 0.5;
    float sinTime2 = sinTime * sinTime;
    fragColor = vec4(sinTime, cosTime, sinTime2, 1.0);
}