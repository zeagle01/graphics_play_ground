#version 330 core


layout(location =0) out vec4 frag_color;
 in vec4 v_color;

void main()
{
    frag_color=v_color;
    //frag_color=vec4(1,0,.5,1.0);
}