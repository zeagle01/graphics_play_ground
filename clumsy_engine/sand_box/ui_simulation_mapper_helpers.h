
#pragma once

#include "imgui.h"
#include "simulator/simulator.h"

template<float min,float max>
struct Imgui_SlideFloat3
{
	bool operator()(vec3f& v, char const* tag)
	{
		return ImGui::SliderFloat3(tag, &v(0), min, max);
	}
};

template<float min,float max>
struct Imgui_SlideFloat
{
	bool operator()(float& v,char const* tag)
	{
		return ImGui::SliderFloat(tag, &v, min, max);
	}
};

struct Imgui_Checkbox
{
	bool operator()(bool& v,char const* tag)
	{
		return ImGui::Checkbox(tag, &v);
	}
};


struct Set_Value
{
	template<typename T,typename SimT>
	void operator()(clumsy_engine::Simulator* sim, const T& v)
	{
		sim->set_value<SimT>(v);
	}
};


struct Add_Remove
{
	template<typename T,typename SimT>
	void operator()(clumsy_engine::Simulator* sim, const T& v)
	{
		if (v)
		{
			sim->add_interaction<SimT>();
		}
		else
		{
			sim->remove_interaction<SimT>();
		}
	}
};


