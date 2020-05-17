#version 330 core


layout(location=0) in vec4 position;
layout(location=0) in vec2 texture_cood;

uniform vec4 u_color;
uniform mat4 u_MVP;

out vec2 v_texture_cood;

void main()
{
    gl_Position=u_MVP*position;
    v_texture_cood=texture_cood;
};