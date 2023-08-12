#include "cuda_runtime.h"

#ifdef __CUDACC__
#define DEVICE_CALLABLE __device__ __host__ 
#else
#define DEVICE_CALLABLE  
#endif



template<typename dT,typename sT>  
struct Copy
{
	dT* dst;
	sT* src;

	DEVICE_CALLABLE void operator()(int i)
	{
		dst[i] = src[i];
	}
};

struct DummyMove
{
	vec3f* pos;
	float a;
	float t;

	DEVICE_CALLABLE inline void operator()(int i)
	{
		pos[i] += a * sin(t) * vec3f { 1.f, 0.f, 0.f };

		//if (i == 1)
		//{
		//	printf(" %f \n", pos[i](0));
		//}
	}
};
