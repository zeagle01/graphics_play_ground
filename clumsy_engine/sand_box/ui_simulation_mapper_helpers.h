
#pragma once

#include "imgui.h"
#include "simulator/simulator.h"
#include <algorithm>


template<typename T,auto ...Init_List>
struct Type_From_Init_List
{
	using type = T;
	static inline const T value = T{ Init_List... };
};

/////////////////imgui wigets

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

//////////////////set value
struct Set_Value_Exponential
{
	template<typename T,typename SimT>
	void operator()(clumsy_engine::Simulator* sim, const T& v)
	{
		float x = std::pow(10.f,v);
		sim->set_value<SimT>(x);
	}
};

struct Set_Value_Logrithm
{
	template<typename T,typename SimT>
	void operator()(clumsy_engine::Simulator* sim, const T& v)
	{
		float x = std::log10(v);
		sim->set_value<SimT>(x);
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




/////////////

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


