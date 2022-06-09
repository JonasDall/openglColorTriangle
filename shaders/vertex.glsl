#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float offset;
out vec3 vertexColor;

void main()
{
    float pi = 3.1415926536;
    float newX = (aPos.x * cos(offset) - aPos.y * sin(offset)) /*+ offset*/;
    float newY = aPos.x * sin(offset) + aPos.y * cos(offset);
    vertexColor = aColor;
    gl_Position = vec4(newX, newY, aPos.z, 1.0);
}