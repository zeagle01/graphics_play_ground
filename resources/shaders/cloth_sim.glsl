#shader_type vertex
#version 330 core

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;

uniform mat4 u_view_projection;

out vec3 frag_normal;
out vec3 frag_position;

void main()
{
	gl_Position = u_view_projection * vec4(v_position, 1.0);
	frag_normal = v_normal;
	//frag_position=v_position;
}



//-------------------------------
#shader_type fragment
#version 330 core

in vec3 frag_normal;
in vec3 frag_position;

uniform vec3 u_eye_pos;
uniform vec3 u_light_pos;
uniform vec3 u_obj_color;
uniform vec3 u_light_color;
uniform float u_ka;
uniform float u_ks;
uniform float u_kd;
uniform float u_specular_steep;

out vec4 color;

void main()
{
	//ambient
	vec3 ambient_color = u_ka * u_light_color * u_obj_color;

	//diffuse
	vec3 light_dir = normalize(u_light_color - frag_position);
	float diffuse = max(dot(frag_normal, light_dir), 0.);
	vec3 diffuse_color = u_kd * diffuse * u_light_color * u_obj_color;

	//specular
	vec3 eye_dir = normalize(u_eye_pos - frag_position);
	vec3 specular_dir = normalize(light_dir + eye_dir);
	float specular = pow(max(dot(specular_dir, frag_normal), 0), u_specular_steep);
	vec3 specular_color = u_ks * specular * u_light_color * u_obj_color;

	color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
}


