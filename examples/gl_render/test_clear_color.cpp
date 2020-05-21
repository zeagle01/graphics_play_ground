
#include "test_clear_color.h"  
#include "renderer.h"

#include "glad/glad.h"

#include "imgui.h"


namespace test
{

	Test_Clear_Color::Test_Clear_Color() :
		m_color{ 1.f,1.0f,0.f,1.0f }
	{
		LOG(INFO) << "clear_color_test " << (void*)this << " construct!";
	}

	Test_Clear_Color::~Test_Clear_Color()
	{
		LOG(INFO) << "clear_color_test " << (void*)this << " destruct!";
	}

	void Test_Clear_Color::on_update(float deltaTime)
	{

	}
	void Test_Clear_Color::on_render()
	{
		GL_Call(glClearColor(m_color[0], m_color[1], m_color[2], m_color[2]));
		GL_Call(glClear(GL_COLOR_BUFFER_BIT));

	}
	void Test_Clear_Color::on_imgui_render()
	{
		ImGui::ColorEdit4("clear color", m_color);

	}

}

