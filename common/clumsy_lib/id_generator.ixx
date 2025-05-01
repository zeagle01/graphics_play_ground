
module;

#include <stdint.h>

export module clumsy_lib:id_generator;


namespace clumsy_lib
{

	export using id = uint64_t;

	export class id_generator
	{
	public:
		static id new_id()
		{
			return s_id++;
		}

	private:
		static uint64_t s_id;
	};

	uint64_t id_generator::s_id = 0;
}
