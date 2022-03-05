
#include "sandbox_2D.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "clumsy_engine/openGL_shader.h"



void Sandbox_2D::on_update(clumsy_engine::Time_Step dt)
{
	m_camara_controller->on_update(dt);

	clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
	clumsy_engine::Render_Command::clear();

	clumsy_engine::Renderer::begin_scene(m_camara_controller->get_camara());

	std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader>(m_shader_plane)->upload_uniform_vec4("u_color", m_plane_color);
	clumsy_engine::Renderer::submit(m_shader_plane, m_vertex_array_plane, glm::mat4(1.f));

	clumsy_engine::Renderer::end_scene();

}
void Sandbox_2D::on_imgui_render(ImGuiContext* imgui_context)
{

	ImGui::SetCurrentContext(imgui_context); //cross lib imgui context do not share ! so set it manully

	ImGui::Begin("Test from demo layer");
	ImGui::Text("hello world from demo layer");
	ImGui::ColorEdit4("plane color", glm::value_ptr(m_plane_color));
	ImGui::End();

}
void Sandbox_2D::on_event(clumsy_engine::Event& e)
{
	m_camara_controller->on_event(e);

}
void Sandbox_2D::on_attach()
{

	m_camara_controller->set_aspect_ratio(800.f / 600.f);


	m_positions_plane = {
		0.,0,0,
		1,0,0,
		1,1,0,
		0,1,0
	};

	std::vector<int> triangles{
		0,1,2,
		0,2,3
	};

	//shader

	std::string resources_dir = "../../../resources/";
	m_shader_plane = clumsy_engine::Shader::create(resources_dir + "shaders/flat_plane_2d.glsl");
	auto ogl_shader = std::dynamic_pointer_cast<clumsy_engine::OpenGL_Shader> (m_shader_plane);


	//gl data stuff
	m_vertex_array_plane = clumsy_engine::Vertex_Array::create();
	std::string position_name_in_shader = "position";
	int v_num = m_positions_plane.size() / 3;

	m_vertex_array_plane->add_vertex_attribute(ogl_shader->get_id(), clumsy_engine::Shader_Data_Type::Float3, position_name_in_shader);
	m_vertex_array_plane->set_vertex_attribute_data(position_name_in_shader, m_positions_plane.data(), v_num);

	clumsy_engine::Ref<clumsy_engine::Index_Buffer> index_buffer = clumsy_engine::Index_Buffer::create(triangles.data(), triangles.size());
	m_vertex_array_plane->set_index_buffer(index_buffer);

}

void Sandbox_2D::on_detach()
{

}
