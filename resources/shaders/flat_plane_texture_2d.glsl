

#shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texture_coodinate;
uniform mat4 u_view_projection;

out vec2 v_texture_coodinate;
out vec4 v_color;
void main()
{
	v_texture_coodinate = texture_coodinate;
	v_color = color;
	gl_Position = u_view_projection * vec4(position, 1.0);
}


//-------------------------------
#shader_type fragment
#version 330

layout(location = 0) out vec4 f_color;

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_tilingFactor;
in vec2 v_texture_coodinate;
in vec4 v_color;

void main()
{
	//f_color = texture(u_texture, v_texture_coodinate * u_tilingFactor) * u_color;
	f_color = v_color;
}
