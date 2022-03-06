

#shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coodinate;
uniform mat4 u_view_projection;
uniform mat4 u_model_matrix;

out vec2 v_texture_coodinate;
void main()
{
	v_texture_coodinate = texture_coodinate;
	gl_Position = u_view_projection * u_model_matrix * vec4(position, 1.0);
}


//-------------------------------
#shader_type fragment
#version 330

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_color;
in vec2 v_texture_coodinate;

void main()
{
	color = texture(u_texture, v_texture_coodinate * 10.f) * u_color;
}
