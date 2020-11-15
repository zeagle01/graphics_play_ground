
#pragma once


#include <vector>
#include <string>
#include <functional>
#include "glog/logging.h"


namespace test
{
	class Test
	{

	public:
		Test() {}
		virtual ~Test() {}

		virtual void on_update(float deltaTime) {}
		virtual void on_render() {}
		virtual void on_imgui_render() {}

	};


	class Test_Menu :public Test
	{

	public:
		Test_Menu(std::unique_ptr<Test>& current_test);
		virtual ~Test_Menu();

		virtual void on_update(float deltaTime)override;
		virtual void on_render()override;
		virtual void on_imgui_render() override;
		template<typename T>
		void register_test(std::string name)
		{

			LOG(INFO) << "register test " << name;

			m_tests.push_back(std::make_pair(name, []() {return new T();}) );
		}
	private:
		std::unique_ptr<Test>& m_current_test;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
	};
}