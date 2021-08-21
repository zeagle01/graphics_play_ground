#pragma once


#include "clumsy_lib/class_reflection.h"
#include "simulation_data.h"
#include "simulator/simulator.h"
#include "matrix_math/matrix_math.h"
#include "ui_simulation_mapper_helpers.h"
#include "simulator/simulation_data.h"
#include "simulator/simulation_interactions.h"


#include <memory>


#define ADD_SIM_DATA(type_name) \
		ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(type_name,clumsy_engine::data::)\

#define ADD_SIM_INTERACTION(type_name) \
		ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(type_name,clumsy_engine::interaction::)\

struct Simulation_Data_Group
{
	ADD_SIM_DATA(Time_Step);
	ADD_SIM_DATA(Gravity_Acceleration);
	ADD_SIM_DATA(Stretch_Stiff);
	ADD_SIM_DATA(Mass_Density);

	//interaction
	ADD_SIM_INTERACTION(Gravity);
	ADD_SIM_INTERACTION(Spring_Stretch);
	ADD_SIM_INTERACTION(Inertial);
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
	ADD_MAPPER_RECORD(Gravity_Acceleration, Set_Value, CE_WRAP(Imgui_SlideFloat3<-10.f, 10.0f>), vec3f);

	ADD_MAPPER_RECORD(Spring_Stretch, Add_Remove, Imgui_Checkbox, bool);
	ADD_MAPPER_RECORD(Gravity, Add_Remove, Imgui_Checkbox, bool);
	ADD_MAPPER_RECORD(Inertial, Add_Remove, Imgui_Checkbox, bool);
};
