
#pragma once

#include "imgui.h"

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


struct Set_Value
{
	template<typename T,typename SimT>
	void operator()(clumsy_engine::Simulator* sim, const T& v)
	{
		sim->set_value<SimT>(v);
	}
};
