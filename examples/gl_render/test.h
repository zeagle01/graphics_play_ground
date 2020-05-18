
#pragma once



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
}