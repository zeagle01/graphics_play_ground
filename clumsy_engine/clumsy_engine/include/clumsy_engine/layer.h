
#pragma once


#include <string>
#include "time_step.h"

struct  ImGuiContext;

namespace clumsy_engine
{
	class Event;

	class Layer
	{
	public:
		Layer(const std::string& name = "layer");
		virtual ~Layer();

		virtual void on_attach() {};
		virtual void on_detach() {};
		virtual void on_update(Time_Step dt) {};
		virtual void on_imgui_render(ImGuiContext* imgui_context) {};
		virtual void on_event(Event& e) {}


		//for debug only,never use in real logic
		inline const std::string& get_name() const { return m_name; }
	private:
		std::string m_name;

	};


}