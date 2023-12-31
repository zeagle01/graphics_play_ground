module;


#include <limits>
#include <algorithm>

export module geometry_lib:aabb;

namespace geometry
{


	template<typename T>
	class single_value_wrapper
	{
	public:
		single_value_wrapper() = default;
		single_value_wrapper(const T& v) : value(v) {};
		T& operator()(int i)
		{
			return  value;
		}

		const T& operator()(int i) const
		{
			return  value;
		}

	private:
		T value;
	};


	export
		template<typename element_type, int N, typename vec >
	class AABB
	{
	public:

		AABB()
		{
			for (int i = 0; i < N; i++)
			{
				upper(i) = std::numeric_limits<element_type>::lowest();
				lower(i) = std::numeric_limits<element_type>::max();
			}

		}

		AABB(const vec& up, const vec& down) :upper(up), lower(down) {}

		AABB& operator+=(const vec& point)
		{
			for (int i = 0; i < N; i++)
			{
				upper(i) = std::max(point(i), upper(i));
				lower(i) = std::min(point(i), lower(i));
			}
			return *this;

		}

		bool intersect(const AABB& other) const
		{
			bool ret = false;
			for (int i = 0; i < N; i++)
			{
				if (other.upper(i) > this->lower(i)
				 && this->upper(i) > other.lower(i))
				{
					return true;
				}
			}

			return ret;
		}

	private:
		vec upper;
		vec lower;
	};


	export 
	using AABB1f = AABB<float, 1, single_value_wrapper<float>>;

	export 
	template< template<typename, int> typename vec>
	using AABB2f = AABB<float, 2, vec<float, 2>>;

	export 
	template< template<typename, int> typename vec>
	using AABB3f = AABB<float, 3, vec<float, 3>>;


}