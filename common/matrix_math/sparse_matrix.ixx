
module;

#include <vector>
#include <functional>
#include <map>

export module matrix_math;


export 
template<typename T>
class sparse_matrix
{

public:

	struct r_c
	{
		int row;
		int col;
	};

	struct r_c_v
	{
		const T& value;
		int row;
		int col;
	};

	struct c_v
	{
		const T& value;
		int col;
	};

public:

	void reserve_space(int num_nz, std::function<r_c(int)> get_row_col)
	{

	}

	using sub_mat = std::function<const T& (int, int)>;
	using sub_mats = std::function<sub_mat(int)>;

	std::function<void(sub_mats)> get_write_loop(int stencil_num, std::function<std::vector<int>(int)> get_stencil)
	{
		auto ret = [&, get_stencil](sub_mats get_stencil_m)
			{
				for (int si = 0; si < stencil_num; si++)
				{
					auto stencil_m = get_stencil_m(si);
					std::vector<int> stencil = get_stencil(si);
					for (int i = 0; i < stencil.size(); i++)
					{
						int vi = stencil[i];
						for (int j = 0; j < stencil.size(); j++)
						{
							int vj = stencil[j];
							m_mat[vi][vj] = stencil_m(i,j);
						}
					}
				}

			};
		return ret;
	}

	void set_values(std::function<r_c_v( int)> get_value, int num_nz)
	{
		for (int i = 0; i < num_nz; i++)
		{
			auto nz = get_value(i);
			m_mat[nz.row][nz.col] = nz.value;
		}

	}

	void set_values(int row_n, std::function<int(int)> get_row_nz, std::function<c_v(int, int)> get_value)
	{
		for (int row = 0; row < row_n; row++)
		{
			for (int j = 0; j < get_row_nz(row); j++)
			{
				c_v col_value = get_value(row, j);

				m_mat[row][col_value.col] = col_value.value;
			}
		}
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
	std::map<int, std::map<int, float>> m_mat;
};
