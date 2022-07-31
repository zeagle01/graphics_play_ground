
#pragma once

#include <vector>

namespace clumsy_engine
{

	template<typename  T> 
	struct CPU_Data_IO
	{
		static void upload(clumsy_lib::Raw_Pointer<T>& device_pointer, const T& host_data) { };
		static void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer) {  };
	};

	template<typename  E> 
	struct CPU_Data_IO<std::vector<E>>
	{
		using T = std::vector<E>;
		static void upload(clumsy_lib::Raw_Pointer<T>& device_pointer, const T& host_data) 
		{
			device_pointer.ptr = const_cast<E*>(host_data.data());
			device_pointer.size = host_data.size();
		};

		static void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer)
		{
			if ( host_data.data() == device_pointer.ptr)
			{
				//just do nothing, because pointer point to same address
			}
			else
			{
				if (host_data.size() < device_pointer.size)
				{
					host_data.resize(device_pointer.size);
				}
				for (int i = 0; i < device_pointer.size; i++)
				{
					host_data[i] = device_pointer.ptr[i];
				}
			}
		};
	};

	class CPU_Serial
	{
	public:
		template<typename Kernel>
		void loop(Kernel k, int size)
		{
			for (int i = 0; i < size; i++)
			{
				//printf(" in cpu %d \n", i);
				k(i);
			}

		};

		template<typename T>
		void upload(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
			CPU_Data_IO<T>::upload(device_ptr, host_data);
		};

		template<typename T>
		void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer)
		{
			CPU_Data_IO<T>::download(host_data, device_pointer);
		};

		template<typename T>
		void free(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
		};

		template<typename T>
		void allocate(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
		};
	};

}