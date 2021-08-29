
#pragma once

#include <vector>
#include <functional>

namespace clumsy_engine
{

	template<typename T>
	struct CSR
	{
		std::vector<T> values;
		std::vector<int> I;
		std::vector<int> J;
	};



	template<typename T>
	CSR<T> to_CSR(const std::vector<std::vector<T>>& values, const std::vector<std::vector<int>>& adjacent_index)
	{
		CSR<T> ret;
		ret.I.resize(adjacent_index.size() + 1);
		const int guess_average_adjacent_num = 6;
		ret.J.reserve(adjacent_index.size() * guess_average_adjacent_num);
		ret.values.reserve(adjacent_index.size() * guess_average_adjacent_num);

		ret.I[0] = 0;
		for (int i = 0; i < adjacent_index.size(); i++)
		{
			ret.I[i + 1] = ret.I[i] + adjacent_index[i].size();
			for (int j = 0; j < adjacent_index[i].size(); j++)
			{
				ret.J.push_back(adjacent_index[i][j]);
				ret.values.push_back(values[i][j]);
			}
		}
		ret.J.shrink_to_fit();
		ret.values.shrink_to_fit();
		return ret;
	}

	template<typename T>
	CSR<T> to_CSR(int non_zero_num, int v_num, std::function<const T& (int)> get_value, std::function<std::array<int, 2>(int)> get_index)
	{
		std::vector<std::vector<int>> adjacent_index(v_num);
		std::vector<std::vector<T>> adjacent_value(v_num);
		for (int i = 0; i < non_zero_num; i++)
		{
			const auto& value = get_value(i);
			auto row_col = get_index(i);

			adjacent_index[row_col[0]].push_back(row_col[1]);
			adjacent_value[row_col[0]].push_back(value);
		}
		return to_CSR(adjacent_value, adjacent_index);
	}


	template<typename T>
	class IJV_To_CSR
	{
	public:
		void add(int i, int j, const T& v) 
		{
			if (i + 1 > m_adjacent_index.size())
			{
				m_adjacent_index.resize(i + 1);
				m_values.resize(i + 1);
			}

			m_adjacent_index[i].push_back(j);
			m_values[i].push_back(v);
		}

		const CSR<T>& get_CSR() const 
		{ 
			return to_CSR(m_values,m_adjacent_index); 
		}

	private:
		std::vector<std::vector<T>> m_values;
		std::vector<std::vector<int>> m_adjacent_index;
	};
}




