
#shader_type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
out vec3 v_position;
out vec3 v_color;

uniform mat4 u_view_projection;
uniform mat4 u_model_matrix;

void main()
{

	v_position = a_position;
	v_color = a_color;
	gl_Position = u_view_projection * u_model_matrix * vec4(a_position, 1.0);
}


//-------------------------------
#shader_type fragment
#version 330 core
in vec3 v_position;
in vec3 v_color;
out vec4 color;

void main()
{
	color = vec4(v_position + v_color, 1.0);
	//color=vec4(v_position,1.0);
}
