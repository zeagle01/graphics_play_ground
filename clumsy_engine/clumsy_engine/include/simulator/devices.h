
#include "clumsy_lib/class_reflection.h"
#include "CPU_serial.h"
#include "CUDA.h"
#include <variant>
#include <vector>

namespace clumsy_engine
{

	struct Device
	{
		ADD_EXIST_TYPE_TO_GROUP(CPU_Serial);
		ADD_EXIST_TYPE_TO_GROUP(CUDA);

		// to be move out
		std::vector<std::variant<CPU_Serial, CUDA>>  devices;

		template<typename Kernel>
		void loop(const Kernel& k, int size)
		{
			if (devices.empty())
			{
				devices.push_back(CPU_Serial{});
				devices.push_back(CUDA{});
			}

			std::visit(
				[&](auto& device) 
				{
					return device.loop(k, size);
				},
				devices[m_current_type]);

		};


		int m_current_type = 1;
	};


}
