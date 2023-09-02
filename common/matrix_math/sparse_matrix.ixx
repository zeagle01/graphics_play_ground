
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
