
#include <iostream>
#include <memory>

#include "imgui.h"
#include "clumsy_engine/clumsy_engine.h"

class Layer_Demo :public clumsy_engine::Layer
{
public:
	Layer_Demo() :clumsy_engine::Layer("demo_layer") {}

	virtual void on_attach() {};
	virtual void on_detach() {};
	virtual void on_update() 
	{
		 
		if (clumsy_engine::Input::is_key_pressed(CE_KEY_TAB))
		{
			CE_TRACE("layer {0} detect tab pressed on update", (void*)this);
		}
	};
	virtual void on_event(clumsy_engine::Event& e) 
	{
		//CE_TRACE("layer {0} handle {1} ", (void*)this, e.to_string());
	}

	virtual void on_imgui_render() 
	{
//		ImGui::Begin("Test");
//		ImGui::Text("hello world");
//		ImGui::End();
	}

};


class SanBox_App:public clumsy_engine::Application
{
	public:
		//SanBox_App() 
		//{
		//}

};



std::unique_ptr<clumsy_engine::Application> clumsy_engine::create_application()
{
	clumsy_engine::Log::get_core_logger()->trace("create app");

	std::unique_ptr<clumsy_engine::Application> app = std::make_unique<SanBox_App>(); 

	std::shared_ptr<clumsy_engine::Layer> layer = std::make_shared<Layer_Demo>();
	app->push_layer(layer);

//	layer = std::make_shared<clumsy_engine::Imgui_Layer>();
//	app->push_overlay(layer);

	return app;
}


