#version 330 core


layout(location =0) out vec4 frag_color;
 in vec4 v_color;
 in vec2 v_texture_cood;
 in float v_texture_unit;

uniform sampler2D u_textures[2];

void main()
{

    int texturei=int(v_texture_unit);
    vec4 texture_color=texture(u_textures[texturei],v_texture_cood);
    frag_color=0.5*v_color+0.5*texture_color;
    //frag_color=vec4(v_texture_unit,v_texture_unit,v_texture_unit,1.0);
}