
#pragma once

#include "imgui.h"
#include "simulator/simulator.h"
#include "clumsy_lib/type_list.h"
#include <algorithm>
#include <string>


template<typename T,auto ...Init_List>
struct Type_From_Init_List
{
	using type = T;
	static inline const T value = T{ Init_List... };
};





/////////////////slider

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

struct Append_Name
{
	template<typename T>
	static void apply(std::string& items, int& size)
	{
		std::string item_name = std::string(typeid(T).name());
		auto pos = item_name.find_last_of(":");
		pos = pos == std::string::npos ? 0 : pos;
		item_name = item_name.substr(pos + 1, item_name.size() - pos - 1);
		items += item_name + '\0';
		size++;
	}
};

template<typename Morphism_T>
struct Imgui_Combobox
{
	Imgui_Combobox() 
	{
		using Morphism_Sub_Type_List = clumsy_lib::extract_member_type_list_t<Morphism_T>; 
		clumsy_lib::for_each_type<Morphism_Sub_Type_List, Append_Name>(m_items_str, m_item_count);
	}

	bool operator()(int& v, char const* tag)
	{
		return ImGui::Combo(tag, &v, m_items_str.c_str(), m_item_count);
	}

	std::string m_items_str;
	int m_item_count = 0;

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



/// add remove type
struct Add_Remove
{
	template<typename UI_T,typename SimT>
	void operator()(clumsy_engine::Simulator* sim, const UI_T& v)
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


///  set type
template<typename Base_T>
struct Set_Type
{
	template<typename SimT>
	static void apply(int type_index, clumsy_engine::Simulator* sim, const int& v)
	{
		if (type_index == v)
		{
			sim->set_morphism_type<Base_T, SimT>();
		}
	};

};


struct Set_Morphism_Type
{
	template<typename T,typename Morphism_T>
	void operator()(clumsy_engine::Simulator* sim, const T& v)
	{
		using Morphism_Sub_Type_List = clumsy_lib::extract_member_type_list_t<Morphism_T>; 
		clumsy_lib::For_Each_Type_With_Index < Morphism_Sub_Type_List, Set_Type<Morphism_T>>::apply(sim, v);
	}
};
