
#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/type_map.h"
#include "CPU_serial.h"
#include "CUDA.h"
#include <variant>
#include <vector>

namespace clumsy_engine
{
	struct Device_Base;

	struct Device
	{
		ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(CPU_Serial, clumsy_engine::);
		ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(CUDA, clumsy_engine::);
		//ADD_EXIST_TYPE_TO_GROUP(CUDA);

		using base_type = Device_Base;

	};


	template<typename tl>
	struct Variant_Holder;

	template<typename ...P>
	struct Variant_Holder<clumsy_lib::type_list<P...>>
	{
		using type = std::variant<P...>;
	};



	struct Device_Base
	{
		using device_types = clumsy_lib::extract_member_type_list_t<Device>;
		Variant_Holder<device_types>::type device;
		using new_type_fn = clumsy_lib::New_Type_To_Variant;

		template<typename Kernel>
		void loop(const Kernel& k, int size)
		{

			std::visit(
				[&](auto& device)
				{
					return device.loop(k, size);
				},
				device);

		};
	};



}
