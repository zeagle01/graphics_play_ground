
#pragma once

#include <vector>
#include <functional>
#include "matrix_math/matrix_math.h"

using namespace matrix_math;

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
	CSR<T> to_CSR(const std::vector<std::vector<T>>& _values, const std::vector<std::vector<int>>& _adjacent_index)
	{
		//reduce duplicate
		std::vector<std::vector<T>> values(_adjacent_index.size());
		std::vector<std::vector<int>> adjacent_index(_adjacent_index.size());
		for (int i = 0; i < _adjacent_index.size(); i++)
		{
			auto& values_i = values[i];
			auto& adjacent_index_i = adjacent_index[i];
			for (int j = 0; j < _adjacent_index[i].size(); j++)
			{
				int adj_I = _adjacent_index[i][j];
				const auto& adj_value = _values[i][j];
				int pos = std::find(adjacent_index_i.begin(), adjacent_index_i.end(), adj_I) - adjacent_index_i.begin();
				if (pos >= 0 && pos < adjacent_index_i.size())
				{
					values_i[pos] += adj_value;
				}
				else if (pos == adjacent_index_i.size())
				{
					adjacent_index_i.push_back(adj_I);
					values_i.push_back(adj_value);
				}
				else
				{
					printf("unexpected %d\n", pos);
				}
			}

		}


		//collect
		CSR<T> ret;
		ret.I.resize(adjacent_index.size() + 1);
		const int guess_average_adjacent_num = 6;
		ret.J.reserve(adjacent_index.size() * guess_average_adjacent_num);
		ret.values.reserve(adjacent_index.size() * guess_average_adjacent_num);

		ret.I[0] = 0;
		for (int i = 0; i < adjacent_index.size(); i++)
		{
			ret.I[i + 1] = ret.I[i] + adjacent_index[i].size();

			int rowBegin = ret.J.size();
			ret.J.push_back(i);
			ret.values.push_back(T{});
			for (int j = 0; j < adjacent_index[i].size(); j++)
			{
				int colIndex = adjacent_index[i][j];
				const auto& colValue = values[i][j];
				if (colIndex == i)
				{
					ret.J[rowBegin] = colIndex;
					ret.values[rowBegin] = colValue;
				}
				else
				{
					ret.J.push_back(colIndex);
					ret.values.push_back(colValue);
				}
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

		CSR<T> get_CSR() const 
		{ 
			return to_CSR(m_values,m_adjacent_index); 
		}

	private:
		std::vector<std::vector<T>> m_values;
		std::vector<std::vector<int>> m_adjacent_index;
	};


	template<typename T, int N>
	std::vector<vec<N, T>>  operator+(const std::vector<vec<N, T>>& v0, const std::vector<vec<N, T>>& v1)
	{
		std::vector<vec<N, T>> ret(v0.size());
		for (int i = 0; i < v0.size(); i++)
		{
			ret[i] = v0[i] + v1[i];
		}
		return ret;
	}

	template<typename T, int N>
	std::vector<vec<N, T>> operator-(const std::vector<vec<N, T>>& v0, const std::vector<vec<N, T>>& v1)
	{
		std::vector<vec<N, T>> ret(v0.size());
		for (int i = 0; i < v0.size(); i++)
		{
			ret[i] = v0[i] - v1[i];
		}
		return ret;
	}


	template<typename T, int N>
	std::vector<vec<N, T>> operator*(const CSR<mat<N, N, T>>& A, const std::vector<vec<N, T>>& v)
	{
		std::vector<vec<N, T>> ret(v.size());
		for (int i = 0; i < A.I.size() - 1; i++)
		{
			ret[i] = get_uniform<N, 1, T>(T{ 0 });
			for (int j = A.I[i]; j < A.I[i + 1]; j++)
			{
				int colIndex = A.J[j];
				ret[i] += A.values[j] * v[colIndex];
			}
		}
		return ret;
	}

}




