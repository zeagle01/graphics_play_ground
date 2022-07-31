
#pragma once

#include "clumsy_lib/class_reflection.h"
#include "clumsy_lib/type_map.h"
#include "CPU_serial.h"
#include "CUDA.h"
#include <variant>
#include <vector>

namespace clumsy_engine
{
	struct Device_Base;

#define ADD_EXSITING_DEVICE(x) ADD_EXIST_TYPE_TO_GROUP_WITH_PREFIX(x, clumsy_engine::);


	struct Device
	{
		ADD_EXSITING_DEVICE(CPU_Serial);
		ADD_EXSITING_DEVICE(CUDA);

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

		template<typename T>
		void upload(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
			std::visit(
				[&](auto& device)
				{
					return device.upload(device_ptr, host_data);
				},
				device);
		};

		template<typename T>
		void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer)
		{
			std::visit(
				[&](auto& device)
				{
					return device.download(host_data, device_pointer);
				},
				device);
		};

		template<typename T>
		void free(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
			std::visit(
				[&](auto& device)
				{
					return device.free(device_ptr, host_data);
				},
				device);
		};

		template<typename T>
		void allocate(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
			std::visit(
				[&](auto& device)
				{
					return device.allocate(device_ptr, host_data);
				},
				device);
		};
	};



}
