
namespace clumsy_engine
{

	class CPU_Serial
	{
	public:
		template<typename Kernel>
		void loop(Kernel k, int size)
		{
			for (int i = 0; i < size; i++)
			{
				k(i);
			}

		};

		template<typename T>
		void upload() {};

		template<typename T>
		void download() {};
	};

}