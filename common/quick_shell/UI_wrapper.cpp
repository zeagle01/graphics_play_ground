module;

#include <functional>
#include <unordered_map>
#include <memory> 

module main_window : UI_wrapper;

import : ui_components;


namespace quick_shell
{

	class ui_component_base
	{
	public:
		~ui_component_base() {};
	};


	template<typename ui_com>
	class ui_component_imp;

	template<>
	class ui_component_imp<ui_component::check_box> :public ui_component_base
	{
	public:
		bool is_changed() { return true; }
		const bool& get_value() { return m_value; }
	private:
		bool m_value;
	};

	class ui_panel
	{
	public:
		template<typename  ui_com>
		void add_ui_component(std::function<void( const typename ui_com::type&)> slot)
		{
			auto ui_com_obj = std::make_shared<ui_component_imp<ui_com>>();

			ui_component_imp<ui_component::check_box>& ui_com_obj_tmp = *ui_com_obj;
			auto wraped_slot = [&,slot]() 
			{
				if (ui_com_obj_tmp.is_changed())
				{
					slot(ui_com_obj_tmp.get_value());
				}
			};
			std::vector<std::function<void()>> slots{wraped_slot};
			m_connects.insert({ ui_com_obj,slots });
		}
	private:
		std::unordered_map<std::shared_ptr<ui_component_base>, std::vector<std::function<void()>>> m_connects;

	};


}
