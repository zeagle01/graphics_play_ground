
#include "test.h"
#include "imgui.h"
#include "glog/logging.h"
#include <chrono>
#include <thread>
#include <memory>

namespace test
{

	Test_Menu::Test_Menu(std::unique_ptr<Test>& current_test)
		:m_current_test(current_test)
	{

		LOG(INFO) << "test_menu " << (void*)this << " construct!";
	}

	Test_Menu::~Test_Menu()
	{
		LOG(INFO) << "test_menu " << (void*)this << " destruct!";
	}

	void Test_Menu::on_update(float deltaTime)
	{

	}
	void Test_Menu::on_render()
	{

	}

	void Test_Menu::on_imgui_render()
	{
		for (auto& test : m_tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_current_test.reset(test.second());
				//std::this_thread::sleep_for(std::chrono::seconds(3));
			}
		}

	}

}
