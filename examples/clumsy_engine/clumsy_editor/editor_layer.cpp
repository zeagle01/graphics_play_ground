
#include "editor_layer.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "clumsy_engine/openGL_shader.h"
#include "clumsy_engine/renderer_2D.h"



namespace clumsy_engine
{
	void editor_layer::on_update(clumsy_engine::Time_Step dt)
	{
		m_camara_controller->on_update(dt);

		clumsy_engine::Renderer_2D::reset_statistic();

		m_frame_buffer->bind();

		clumsy_engine::Render_Command::set_clear_color({ 0.2, 0.1, 0.3, 0.2 });
		clumsy_engine::Render_Command::clear();


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

		m_frame_buffer->unbind();


	}

	void editor_layer::on_imgui_render(ImGuiContext* imgui_context)
	{

		ImGui::SetCurrentContext(imgui_context); //cross lib imgui context do not share ! so set it manully


		//docking space 

		static bool dock_space_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dock_space_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }

				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, &dock_space_open != NULL))
					dock_space_open = false;
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					clumsy_engine::Application::get_singleton().close();
				}

				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		{
			ImGui::Begin("Test from demo layer");
			ImGui::Text("hello world from demo layer");

			ImGui::Text("render statistic:");
			ImGui::Text("draw call: %d", clumsy_engine::Renderer_2D::get_statistic().num_draw_calls);
			ImGui::Text("num quad: %d", clumsy_engine::Renderer_2D::get_statistic().num_quad);
			ImGui::Text("number of vertex: %d", clumsy_engine::Renderer_2D::get_statistic().get_vertex_num());
			ImGui::Text("number of triangles: %d", clumsy_engine::Renderer_2D::get_statistic().get_triangle_num());

			ImGui::ColorEdit4("plane color", glm::value_ptr(m_plane_color));

			ImGui::End();


			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("viewport");
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (viewportSize[0] != m_view_port_size[0] || viewportSize[1] != m_view_port_size[1])
			{
				m_view_port_size[0] = viewportSize[0];
				m_view_port_size[1] = viewportSize[1];

				m_frame_buffer->resize(m_view_port_size[0], m_view_port_size[1]);

				m_camara_controller->on_resize(m_view_port_size[0], m_view_port_size[1]);

			}
			int32_t textureId = m_frame_buffer->get_color_attatchment_render_id();
			ImGui::Image((void*)textureId, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}


		ImGui::End();

	}

	void editor_layer::on_event(clumsy_engine::Event& e)
	{
		m_camara_controller->on_event(e);

	}
	void editor_layer::on_attach()
	{

		m_camara_controller->set_aspect_ratio(800.f / 600.f);

		std::string resources_dir = "../../../resources/";
		std::string texture_image = resources_dir + "textures/awesomeface.png";
		m_texture = clumsy_engine::Texture_2D::create(texture_image);
		m_frame_buffer = clumsy_engine::frame_buffer::create(
			{
				.width = 1280,
				.height = 720
			}
		);


	}

	void editor_layer::on_detach()
	{

	}

}
