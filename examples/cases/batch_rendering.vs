#version 330 core

layout(location =0) in vec4 position;
layout(location =1) in vec4 color;
layout(location =2) in vec2 texture_cood;
layout(location =3) in float texture_unit;

out vec4 v_color;
out vec2 v_texture_cood;
out float v_texture_unit;

void main()
{
    gl_Position=position;
    v_color=color;
    v_texture_cood=texture_cood;
    v_texture_unit=texture_unit;
}