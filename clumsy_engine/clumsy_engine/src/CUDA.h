

namespace clumsy_engine
{

	class CUDA
	{
	public:
		template<typename Kernel>
		void loop(Kernel k, int size);

		template<typename T>
		void upload() {};

		template<typename T>
		void download() {};
	};

}
