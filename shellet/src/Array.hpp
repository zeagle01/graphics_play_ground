


#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <vector>

#include "predef_types.h"

template<typename T>
class Array {




public:
	void resize(int size,T value) {
		data.resize(size, value);
	}

	void resize(int size) {
		data.resize(size);
	}

	bool empty() {
		return data.empty();
	}

	template<int dim,typename continuousT>
	void assignFromContinuousStorage(const continuousT* b, const int size) {

		data.resize(size / dim);
		for (int i = 0; i < size / dim; i++) {
			for (int j = 0; j < dim; j++) {
				data[i][j] = b[i * dim + j];
			}
		}

	}

	template<int dim,typename continuousT>
	std::vector<continuousT> convert_2_std_vector() {
		return 	std::vector<continuousT>(&data[0][0], &data[0][0] + size() * dim);
	}

	int size()const  {
		return data.size();
	}

	T operator[](const int i) const {
		return data[i];
	}

	T& operator[](const int i)  {
		return data[i];
	}


private:
	std::vector<T> data;
};





typedef Array<v3f> a3f;
typedef Array<v3i> a3i;

#endif