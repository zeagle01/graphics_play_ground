#pragma once


#include "clumsy_lib/class_reflection.h"
#include "simulation_data.h"
#include "simulator/simulator.h"
#include "matrix_math/matrix_math.h"
#include "ui_simulation_mapper_helpers.h"


#include <memory>


#define ADD_SIM_DATA(type_name) \
		ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(type_name,clumsy_engine::data::)\

#define ADD_SIM_INTERACTION(type_name) \
		ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(type_name,clumsy_engine::data::)\

struct Simulation_Data_Group
{
	ADD_SIM_DATA(Time_Step);
	ADD_SIM_DATA(Gravity);
	ADD_SIM_DATA(Stretch_Stiff);
	ADD_SIM_DATA(Mass_Density);

	//interaction
	//ADD_EXIST_TYPE_TO_GROUP_WITH_SIM_PREFIX(Spring_Stretch);
};



class Simulation_Data_Mapper_Base
{
public:
	virtual void  operator()(clumsy_engine::Simulator* sim) = 0;
};

template<typename Setter, typename Getter, typename UI_Type, char const* UI_tag, typename Sim_Type >
class Simulation_Data_Mapper :public Simulation_Data_Mapper_Base
{
public:
	void  operator()(clumsy_engine::Simulator* sim) override
	{
		auto changed = m_get_fn(m_ui_value, UI_tag);
		if (changed)
		{
			m_set_fn.operator()<UI_Type,Sim_Type>(sim, m_ui_value);
		}
	}
private:
	UI_Type m_ui_value;
	Setter m_set_fn;
	Getter m_get_fn;
};


#define ADD_MAPPER_RECORD(type_name,set_value,get_value,ui_type) \
	static constexpr char tag_##type_name[] = #type_name;\
	ADD_TYPE_TO_GROUP(type_name, Simulation_Data_Mapper, set_value, get_value, ui_type,tag_##type_name, Simulation_Data_Group::type_name);

struct Mapper_Records
{
	ADD_MAPPER_RECORD(Time_Step, Set_Value, CE_WRAP(Imgui_SlideFloat<0.001f, 10.0f>), float);
	ADD_MAPPER_RECORD(Mass_Density, Set_Value, CE_WRAP(Imgui_SlideFloat<0.001f, 10.0f>), float);
	ADD_MAPPER_RECORD(Stretch_Stiff, Set_Value, CE_WRAP(Imgui_SlideFloat<0.001f, 1e7f>), float);
	ADD_MAPPER_RECORD(Gravity, Set_Value, CE_WRAP(Imgui_SlideFloat3<-10.f, 10.0f>), vec3f);
};
