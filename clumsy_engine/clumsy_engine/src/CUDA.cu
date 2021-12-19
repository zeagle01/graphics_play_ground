

#include "CUDA.h"
#include "cuda_runtime.h" 
#include <iostream>

#include "raw_pointer_kernels.h"


namespace clumsy_engine
{

	template< typename Kernel > __global__ void cuda_for_loop(Kernel k, int size)
	{
		int tid = blockIdx.x * blockDim.x + threadIdx.x;
		if (tid < size)
		{
			k(tid);
		}
	}

	template<typename Kernel>
	void CUDA::loop(Kernel k, int size)
	{
		int tpb = 128;
		int bpg = (size - 1) / tpb + 1;
		cuda_for_loop << <bpg, tpb >> > (k, size);
		//cuda_for_loop << <1, 1 >> > (k, size);
	}

	template void CUDA::loop<Copy<float, float>>(Copy<float, float> k, int size);
}


