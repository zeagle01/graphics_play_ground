
module;

#include "clumsy_lib/dynamic_type_map.h" 
#include "clumsy_lib/type_map.h" 

#include <algorithm>
#include <cmath>


module sim_lib : simulator_imp;

import :sim_data;

namespace sim_lib
{

	class simulator_imp
	{
	public:
		simulator_imp()
		{
			m_datas.add_or_replace<sim_data::positions>();
		}

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

			int vNum = pos.size() / 3;
			for (int i = 0; i < vNum; i++)
			{
				pos[i * 3 + 0] += amp * std::sin(t * freq);
			}

			t += step;
		}

	private:
		clumsy_lib::dynamic_type_map m_datas;

		float t = 0.f;
	};
}