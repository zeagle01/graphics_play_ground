#include "cuda_runtime.h"

#ifdef __CUDACC__
#define DEVICE_CALLABLE __device__ inline
#else
#define DEVICE_CALLABLE  inline
#endif



template<typename dT,typename sT>  
struct Copy
{
	dT* dst;
	sT* src;

	DEVICE_CALLABLE void operator()(int i)
	{
		//dst[i] = src[i];
	}
};
