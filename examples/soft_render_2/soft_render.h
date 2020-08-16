



#pragma once

#include <initializer_list> 


namespace soft_render_2
{
	///vector
	template<size_t N,typename T>
	class vec
	{
		T data[N]; 
	public:
		vec() {}
		vec(const vec& other) { *this = other; }
		vec(const std::initializer_list<T>& il)
		{
			int i = 0;
			for (auto it = il.begin(); it != il.end(); it++)
			{
				assert(i < N);
				data[i] = *it;
				i++;
			}
		}

		vec& operator=(const vec& other) { for (int i = 0; i < N; i++) { data[i] = other[i]; } return *this; }

		T& operator[](size_t i) { return data[i]; }

		const T& operator[](size_t i) const { return data[i]; }

		static vec full(T value) { vec ret; for (int i = 0; i < N; i++) { ret[i] = value; } return ret; }

	};

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



	//////////////////soft renderer
	class Soft_Renderer
	{
	public: 
		int* get_result() { return nullptr; }
		void set_vertex_shader() {}
		void render(){}

		void draw_line() {  }
		void draw_triangle() {}
	};





	using vec3f = vec<3, float>;
	using vec4f = vec<4, float>;

}