
#pragma once

#include <initializer_list> 
#include <functional>
#include <map>


namespace soft_render
{


#define VEC_BASIC()\
		vec() {} \
		vec(const vec& other) { *this = other; } \
		vec(const std::initializer_list<T>& il) { int i = 0; for (auto it = il.begin(); it != il.end(); it++) { assert(i < size); data[i] = *it; i++; } } \
		vec& operator=(const vec& other) { for (int i = 0; i < size; i++) { data[i] = other[i]; } return *this; } \
		T& operator[](size_t i) { return data[i]; } \
		const T& operator[](size_t i) const { return data[i]; } \


	template<size_t N>
	struct Sizable
	{
		static constexpr size_t size = N;
	};


	///vector
	template<size_t N,typename T>
	class vec:public Sizable<N>
	{
		T data[size]; 
	public:
		VEC_BASIC();


	};

	template<typename T>
	class vec<2, T>:public Sizable<2>
	{
	public:
		union {
			struct { T x; T y; };
			struct { T u; T v; };
			T data[2];
		};
		VEC_BASIC()

	};

	template<size_t N, typename T>
	static inline vec<N, T> uniform_vec(T value) { vec<N, T> ret; for (int i = 0; i < N; i++) { ret[i] = value; } return ret; }

	template<size_t N,typename T>
	static inline vec<N,T> operator+(const vec<N, T>& a, const vec<N, T> b)
	{

		vec<N,T> ret;
		for (int i = 0; i < N; i++)
		{
			ret[i] = a[i] + b[i];
		}
		return ret;
	}

	template<size_t N,typename T>
	static inline bool operator==(const vec<N, T>& a, const vec<N, T> b)
	{

		vec<N,T> ret;
		for (int i = 0; i < N; i++)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}
		return true;
	}





	//type alias
	using vec2f = vec<3, float>;
	using vec3f = vec<3, float>;
	using vec4f = vec<4, float>;
	using vec2i = vec<2, int>;
	using vec3i = vec<3, int>;
	using vec4i = vec<4, int>;








}
