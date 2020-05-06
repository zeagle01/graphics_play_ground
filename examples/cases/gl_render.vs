#version 330 core


layout(location=0) in vec4 position;

uniform vec4 u_color;

void main()
{
    gl_Position=position;
};