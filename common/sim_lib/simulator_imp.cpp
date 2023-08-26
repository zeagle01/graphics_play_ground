
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"

#include "matrix_math/matrix_math.h"


module sim_lib : simulator_imp;

import :sim_data;
import :solver;

namespace sim_lib
{


	template<typename T, int N>
	static void convert(std::vector<std::array<T, N>>& out, const std::vector<matrix_math::mat<N, 1, T>>& in)
	{
		out.resize(in.size());
		for (int i = 0; i < in.size(); i++)
		{
			for (int j = 0; j < N; j++)
			{
				out[i][j] = in[i](j);
			}
		}

	}

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
			//TODO: get_instace<enum,base_class>(enum_value);
			if (!m_solver)
			{
				m_solver = std::make_unique<solver>();
			}

			m_solver->update_data(m_datas);
			m_solver->solve();
			convert(m_datas.get_ref<sim_data::positions>(), m_solver->get_result());
		}

	private:
		using var_list = clumsy_lib::extract_member_type_list_t<sim_data>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		std::unique_ptr<solver> m_solver;

	};
}