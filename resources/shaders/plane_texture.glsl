#shader_type vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture;

uniform mat4 u_view_projection;
uniform mat4 u_model_matrix;

out vec3 v_position;
out vec3 v_color;
out vec2 v_texture;

void main()
{
	v_position = position;
	v_texture = texture;
	gl_Position = u_view_projection * u_model_matrix * vec4(position, 1.0);
}



//-------------------------------



#shader_type fragment
#version 330 core
in vec3 v_position;
in vec2 v_texture;
uniform sampler2D u_texture;
out vec4 color;
void main()
{
	color = texture(u_texture, v_texture);
}

