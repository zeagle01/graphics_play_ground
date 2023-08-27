
module;

#include <vector>

module sim_lib: struct_functors;



namespace sim_lib
{
	struct get_size
	{
		template<typename T>
		static void apply(int& out, const std::vector<T>& in)
		{
			out = in.size();
		}
	};

}
