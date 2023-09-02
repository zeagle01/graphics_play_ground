
module;

#include <vector>
#include <functional>
#include <map>

export module matrix_math;


namespace matrix_math
{
	export template<typename T>
	class sparse_matrix
	{

	public:

		struct r_c
		{
			int row;
			int col;
		};

	public:

		void reserve_space(int num_nz, std::function<r_c(int)> get_row_col)
		{

		}

		using get_nz_t = std::function<T& (int si, int i, int j)>;
		using get_stencil_nz_t = std::function<void(get_nz_t, int)>;

		std::function<void(get_stencil_nz_t)> get_write_loop(int stencil_num, std::function<std::vector<int>(int)> get_stencil)
		{

			get_nz_t get_nz_fn = [&,get_stencil](int si, int i, int j)->T&
				{
					std::vector<int> stencil = get_stencil(si);
					int vi = stencil[i];
					int vj = stencil[j];
					return m_mat[vi][vj];
				};


			auto ret = [&,get_nz_fn, stencil_num, get_stencil](std::function<void(get_nz_t, int)> fn)
				{
					for (int si = 0; si < stencil_num; si++)
					{
						fn(get_nz_fn, si);
					}

				};
			return ret;
		}


		void for_each_nz(std::function<void(int row, int col, const T& v)> fn)
		{
			for (const auto& rowIt : m_mat)
			{
				int row = rowIt.first;
				for (const auto& colIt : m_mat[row])
				{
					int col = colIt.first;
					const T& value = colIt.second;
					fn(row, col, value);
				}
			}
		}

	private:
		std::map<int, std::map<int, T>> m_mat;
	};

}
