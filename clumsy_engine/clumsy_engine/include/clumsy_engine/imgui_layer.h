


#pragma once

#include "layer.h"




namespace clumsy_engine
{
	class Layer;
	class Event;

	class Imgui_Layer :public Layer
	{
	public:
		Imgui_Layer();
		~Imgui_Layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void on_update() override;
		virtual void on_event(Event& e) override;

	};

}