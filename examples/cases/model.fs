#version 330 core
in vec2 tex_cood;
out vec4 frag_color;

uniform sampler2D texture_diffuse1;

void main(){

    frag_color=vec4(1,0.5,0.2,1);
	//frag_color=vec4(texture(texture_diffuse1,tex_cood));


}