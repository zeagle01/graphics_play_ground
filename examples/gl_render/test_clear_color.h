

#pragma once


#include "test.h"

namespace test {

	class Test_Clear_Color : public Test
	{
	public:
		Test_Clear_Color();
		virtual ~Test_Clear_Color();

		virtual void on_update(float deltaTime) override;
		virtual void on_render() override;
		virtual void on_imgui_render() override;

	private:
		float m_color[4];
	};

}