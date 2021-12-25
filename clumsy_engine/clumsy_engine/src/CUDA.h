
#include  "cuda_runtime.h"
#include "clumsy_lib/raw_pointer.h"
#include <vector>

namespace clumsy_engine
{



	template<typename  T> 
	struct Cuda_Data_IO
	{
		static void upload(clumsy_lib::Raw_Pointer<T>& device_pointer, const T& host_data) { };
		static void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer) {  };
	};

	template<typename  E> 
	struct Cuda_Data_IO<std::vector<E>>
	{
		using T = std::vector<E>;
		static void upload(clumsy_lib::Raw_Pointer<T>& device_pointer, const T& host_data) 
		{
			cudaFree(device_pointer.ptr);
			cudaMalloc(&device_pointer.ptr, sizeof(E) * host_data.size());

			cudaMemcpy(device_pointer.ptr, host_data.data(), sizeof(E) * host_data.size(), cudaMemcpyHostToDevice);
			device_pointer.size = host_data.size();
		};

		static void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer) 
		{
			if (host_data.size() < device_pointer.size)
			{
				host_data.resize(device_pointer.size);
			}
			cudaMemcpy(host_data.data(), device_pointer.ptr, sizeof(E) * device_pointer.size, cudaMemcpyDeviceToHost);
		};
	};
		


	class CUDA
	{
	public:
		template<typename Kernel>
		void loop(Kernel k, int size);

		template<typename T>
		void upload(clumsy_lib::Raw_Pointer<T>& device_ptr, const T& host_data)
		{
			Cuda_Data_IO<T>::upload(device_ptr, host_data);
		};

		template<typename T>
		void download(T& host_data, clumsy_lib::Raw_Pointer<T> device_pointer)
		{
			Cuda_Data_IO<T>::download(host_data, device_pointer);
		};
	};

}
