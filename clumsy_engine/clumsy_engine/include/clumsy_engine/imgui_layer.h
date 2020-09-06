


#pragma once

#include "layer.h"






namespace clumsy_engine
{


	class Layer;
	class Event;

	class Mouse_Button_Pressed_Event;
	class Mouse_Button_Released_Event;
	class Mouse_Moved_Event;
	class Mouse_Scrolled_Event;
	class Key_Pressed_Event;
	class Key_Release_Event;
	class Window_Resize_Event;
	class Key_Typed_Event;

	class Imgui_Layer :public Layer
	{
	public:
		Imgui_Layer();
		~Imgui_Layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
	//	virtual void on_update() override;
		virtual void on_imgui_render() override;

		void begin();
		void end();


	};

}