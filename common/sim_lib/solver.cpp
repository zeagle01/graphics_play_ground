
module;

#include "clumsy_lib/static_type_map.h"
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/dependent_propagator.h"

#include "clumsy_lib/field_defines.h"

#include "matrix_math/matrix_math.h"

#include <cmath>

module sim_lib:solver;

import :sim_data;


namespace sim_lib
{
	//
	using vec3 = matrix_math::mat<3, 1, float>;
	using mat3 = matrix_math::mat<3, 3, float>;

	namespace solver_data_update
	{
		struct convert
		{
			template<typename T, int N>
			static void apply(std::vector<matrix_math::mat<N, 1, T>>& out, const std::vector<std::array<T, N>>& in)
			{
				out.resize(in.size());
				for (int i = 0; i < in.size(); i++)
				{
					for (int j = 0; j < N; j++)
					{
						out[i](j) = in[i][j];
					}
				}

			}
		};

	}




	class solver
	{
	public:
		template<typename data_holder>
		void update_data(const data_holder& data, const clumsy_lib::change_status_t& change_status)
		{
			clumsy_lib::dependent_updater<var_list>::apply(m_datas, data, change_status);
		}

		void solve()
		{
			float amp = 0.01f;
			float freq = 1.0f;
			float step = 0.01f;

			auto& pos = m_datas.get_ref<var::positions>();
			int vNum = pos.size();
			for (int i = 0; i < vNum; i++)
			{
				pos[i](0) += amp * std::sin(t * freq);
			}

			t += step;

		}

		const std::vector<vec3>& get_result() const
		{
			return m_datas.get_ref<var::positions>();
		}


		clumsy_lib::adj_list_t compute_dep_graph()
		{
			return clumsy_lib::Dependent_Graph::build<var_list>();
		}

	private:

#define CE_SOLVER_DATA(name,type,update_fn,dep_list,tag_list) CE_ADD_NODE(name, CE_TYPE(type) CE_FIELD(dep_update_fn,update_fn) CE_FIELD(deps,dep_list) CE_FIELD(tags,tag_list))

		template<typename ...T>
		using tl = clumsy_lib::type_list<T...>;
		struct var
		{
			CE_SOLVER_DATA(positions,		std::vector<vec3>,		solver_data_update::convert, tl<sim_data::positions>,	tl<>)

		};

		using var_list = clumsy_lib::extract_member_type_list_t<var>;
		clumsy_lib::Static_Type_Map<var_list> m_datas;

		float t = 0.f;
	};

}
