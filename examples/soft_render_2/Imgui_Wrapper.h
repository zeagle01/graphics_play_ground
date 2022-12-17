
#pragma once

#include <functional>
#include <vector>
#include "mat.h"
#include "type_map.h"


struct GLFWwindow;

namespace soft_render
{

	struct ui_component_base
	{
		void virtual operator()() = 0;
	};

	struct slider_bar_float3 :ui_component_base
	{
		std::string name;
		float min = 0.f;
		float max = 1.f;
		vec3* value;

		void operator()()override;
	};



	class Imgui_Wrapper
	{

	public:
		void init(GLFWwindow* window);
		void update();

		template<typename ui,typename value_t>
		void add_ui_component(const std::string& name, value_t& value)
		{
			auto ui_component = std::make_shared<ui>();
			ui_component->value = &value;
			ui_component->name = name;
			m_ui_components_new.push_back(ui_component);
		}

	private:
		std::vector<std::shared_ptr<ui_component_base>> m_ui_components_new;
	};
}
