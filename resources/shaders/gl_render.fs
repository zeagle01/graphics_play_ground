
#version 330 core

layout(location =0) out vec4 frag_color;

in vec2 v_texture_cood;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main()
{
    vec4 texture_color=texture(u_texture,v_texture_cood);
    frag_color=u_color*texture_color;
    //frag_color=texture_color;
};