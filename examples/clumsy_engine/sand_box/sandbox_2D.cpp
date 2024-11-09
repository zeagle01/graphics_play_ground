
#include "sandbox_2D.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "clumsy_engine/openGL_shader.h"
#include "clumsy_engine/renderer_2D.h"



void Sandbox_2D::on_update(clumsy_engine::Time_Step dt)
{
	m_camara_controller->on_update(dt);

	clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
	clumsy_engine::Render_Command::clear();

	clumsy_engine::Renderer_2D::reset_statistic();

	clumsy_engine::Renderer_2D::begin_scene(m_camara_controller->get_camara());

	int num_of_column = 100;
	float side = 2.f / num_of_column;
	for (int i = 0; i < num_of_column; i++)
	{
		for (int j = 0; j < num_of_column; j++)
		{
			if ((i + j) % 2 == 0)
			{
				clumsy_engine::Renderer_2D::draw_quad
				(
					{
						.position = { i * side,j * side },
						.size = { side, side },
						.rotation = glm::radians(45.f),
						.tint_color = m_plane_color
					}
				);
			}
			else
			{
				clumsy_engine::Renderer_2D::draw_quad
				(
					{
					.position = { i * side,j * side },
					.size = { side, side },
					.tint_color = m_plane_color
					}
				);
			}

		}
	}

	clumsy_engine::Renderer_2D::draw_quad
	(
		{
		.position = { 0.5f, 0.1f },
		.size = { 0.5f,0.5f },
		.rotation = glm::radians(45.f),
		.texture = m_texture,
		.tilling_factor = 10.f,
		.tint_color = {0.9f, 0.8f, 0.9f, 1.0},
		.texture_origin = {0.03f,0.0f},
		.texture_span = {0.15f,0.15f},
		}
	);

	clumsy_engine::Renderer_2D::end_scene();


}
void Sandbox_2D::on_imgui_render(ImGuiContext* imgui_context)
{

	ImGui::SetCurrentContext(imgui_context); //cross lib imgui context do not share ! so set it manully

	ImGui::Begin("Test from demo layer");
	ImGui::Text("hello world from demo layer");

	ImGui::Text("render statistic:");
	ImGui::Text("draw call: %d", clumsy_engine::Renderer_2D::get_statistic().num_draw_calls);
	ImGui::Text("num quad: %d", clumsy_engine::Renderer_2D::get_statistic().num_quad);
	ImGui::Text("number of vertex: %d", clumsy_engine::Renderer_2D::get_statistic().get_vertex_num());
	ImGui::Text("number of triangles: %d", clumsy_engine::Renderer_2D::get_statistic().get_triangle_num());

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

	std::string resources_dir = "../../../resources/";
	std::string texture_image = resources_dir + "textures/awesomeface.png";
	m_texture = clumsy_engine::Texture_2D::create(texture_image);


}

void Sandbox_2D::on_detach()
{

}
