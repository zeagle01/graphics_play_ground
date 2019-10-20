


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

	template<typename continuousT>
	void assignFromContinuousStorage(const continuousT* b, const int size) {

		data.resize(size / 3);
		for (int i = 0; i < size / 3; i++) {
			data[i] = T(b[i*3+0], b[i * 3 + 1], b[i * 3 + 2]);
		}

	}

	template<typename continuousT>
	std::vector<continuousT> convert_2_std_vector() {
		return 	std::vector<continuousT>(&data[0][0], &data[0][0] + size() * 3);
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