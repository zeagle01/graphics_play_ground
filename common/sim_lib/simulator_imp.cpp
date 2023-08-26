
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"

#include <algorithm>
#include <cmath>
#include <array>


module sim_lib : simulator_imp;

import :sim_data;

namespace sim_lib
{

	class simulator_imp
	{
	public:

		template<typename var>
		void set(typename const var::type& data)
		{
			m_datas.get_ref<var>() = data;
		}

		template<typename var>
		const typename var::type& get()
		{
			return m_datas.get_ref<var>();
		}


		void step()
		{
			auto& pos = m_datas.get_ref<sim_data::positions>();

			float amp = 0.01f;
			float freq = 1.0f;
			float step = 0.01f;

			int vNum = pos.size();
			for (int i = 0; i < vNum; i++)
			{
				pos[i][0] += amp * std::sin(t * freq);
			}

			t += step;
		}

	private:
		using var_list = clumsy_lib::extract_member_type_list_t<sim_data>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		float t = 0.f;

	};
}