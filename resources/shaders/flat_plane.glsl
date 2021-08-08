
#shader_type vertex
#version 330 core
layout(location = 0) in vec3 position;

uniform  vec3 u_color;
uniform mat4 u_view_projection;
uniform mat4 u_model_matrix;

out vec3 v_position;
out vec3 v_color;

void main()
{
	//vec3 t_position=u_view_projection*position;
	v_position = position;
	v_color = u_color;
	gl_Position = u_view_projection * u_model_matrix * vec4(position, 1.0);
}


//-------------------------------
#shader_type fragment
#version 330 core
in vec3 v_position;
in vec3 v_color;
out vec4 color;
void main()
{
	color = vec4(v_color, 1.0);
}
