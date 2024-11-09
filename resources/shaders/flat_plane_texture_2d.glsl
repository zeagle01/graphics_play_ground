

#shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texture_coodinate;
layout(location = 3) in float texture_index;
layout(location = 4) in float tilling_factor;

uniform mat4 u_view_projection;

out vec2 v_texture_coodinate;
out vec4 v_color;
out float v_texture_index;
out float v_tilling_factor;
void main()
{
	v_texture_coodinate = texture_coodinate;
	v_color = color;
	v_texture_index = texture_index;
	v_tilling_factor = tilling_factor;

	gl_Position = u_view_projection * vec4(position, 1.0);
}


//-------------------------------
#shader_type fragment
#version 330

layout(location = 0) out vec4 f_color;

uniform sampler2D u_textures[32];

in vec2 v_texture_coodinate;
in vec4 v_color;
in float v_texture_index;
in float v_tilling_factor;

void main()
{
	f_color = texture(u_textures[int(v_texture_index)], v_texture_coodinate * v_tilling_factor) * v_color;
	//f_color = v_color;
}
