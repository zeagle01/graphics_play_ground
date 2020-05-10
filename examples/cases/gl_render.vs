#version 330 core


layout(location=0) in vec4 position;
layout(location=0) in vec2 texture_cood;

uniform vec4 u_color;
out vec2 v_texture_cood;

void main()
{
    gl_Position=position;
    v_texture_cood=texture_cood;
};