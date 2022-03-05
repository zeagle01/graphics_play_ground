

#shader_type vertex
#version 330 core

layout(location = 0) in vec3 position;
uniform mat4 u_view_projection;
uniform mat4 u_model_matrix;

void main()
{
	gl_Position = u_view_projection * u_model_matrix * vec4(position, 1.0);
}


//-------------------------------
#shader_type fragment
#version 330
layout(location = 0) out vec4 color;
uniform vec4 u_color;
void main()
{
	//color = vec4(1.f, 0.f, 0.f, 1.f);
	color = u_color;
}
