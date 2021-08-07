
#include <string>

// /////////////////// sim app /////////////////////////////
static inline std::string sim_app_vertex_src = R"(
			#version 330 core

			layout(location=0 ) in vec3 v_position;
			layout(location=1 ) in vec3 v_normal;

			uniform mat4 u_view_projection;

			out vec3 frag_normal;
			out vec3 frag_position;

			void main()
			{
				gl_Position=u_view_projection*vec4(v_position,1.0);
				frag_normal=v_normal;
				//frag_position=v_position;
			}

		)";

static inline std::string sim_app_fragment_src = R"(
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
				vec3 ambient_color=u_ka*u_light_color*u_obj_color;

				//diffuse
				vec3 light_dir=normalize(u_light_color-frag_position);
				float diffuse=max(dot(frag_normal,light_dir),0.);
				vec3 diffuse_color=u_kd*diffuse*u_light_color*u_obj_color;

				//specular
				vec3 eye_dir=normalize(u_eye_pos-frag_position);
				vec3 specular_dir=normalize(light_dir+eye_dir);
				float specular=pow(max(dot(specular_dir,frag_normal),0),u_specular_steep);
				vec3 specular_color=u_ks*specular*u_light_color*u_obj_color;

				color=vec4(ambient_color+diffuse_color+specular_color,1.0);
				//color=vec4(frag_normal,1.0);
				//color=vec4(1.0,0.,0.,1.0);
			}

		)";





// /////////////////// sand box app /////////////////////////////

// ///////////////////  triangle /////////////////////////////

static inline std::string sand_box_triangle_vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 a_position;
			layout(location=1 ) in vec3 a_color;
			out vec3 v_position;
			out vec3 v_color;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_matrix;

			void main()
			{

				v_position=a_position;
				v_color=a_color;
				gl_Position=u_view_projection*u_model_matrix*vec4(a_position,1.0);
			}

		)";

static inline std::string sand_box_triangle_fragment_src = R"(

			#version 330 core
			in vec3 v_position;
			in vec3 v_color;
			out vec4 color;

			void main()
			{
				color=vec4(v_position+v_color,1.0);
				//color=vec4(v_position,1.0);
			}

		)";


// ///////////////////  plane /////////////////////////////
static inline std::string sand_box_plane_vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 position;

			uniform  vec3 u_color;
			uniform mat4 u_view_projection;
			uniform mat4 u_model_matrix;

			out vec3 v_position;
			out vec3 v_color;

			void main()
			{
				//vec3 t_position=u_view_projection*position;
				v_position=position;
				v_color=u_color;
				gl_Position=u_view_projection*u_model_matrix*vec4(position,1.0);
			}

		)";

static inline std::string sand_box_plane_fragment_src = R"(
			#version 330 core
			in vec3 v_position;
			in vec3 v_color;
			out vec4 color;
			void main()
			{
				color=vec4(v_color,1.0);
			}
		)";



// ///////////////////  texture /////////////////////////////
static inline std::string sand_box_texture_vertex_src = R"(
			#version 330 core
			layout(location=0 ) in vec3 position;
			layout(location=1 ) in vec2 texture;

			uniform mat4 u_view_projection;
			uniform mat4 u_model_matrix;

			out vec3 v_position;
			out vec3 v_color;
			out vec2 v_texture;

			void main()
			{
				v_position=position;
				v_texture=texture;
				gl_Position=u_view_projection*u_model_matrix*vec4(position,1.0);
			}

		)";

static inline std::string sand_box_texture_fragment_src = R"(
			#version 330 core
			in vec3 v_position;
			in vec2 v_texture;
			out vec4 color;
			void main()
			{
				color=vec4(v_texture,0.,1.0);
				//color=vec4(v_position,1.0);
			}
		)";


