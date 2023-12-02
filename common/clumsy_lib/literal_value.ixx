module;



export module clumsy_lib : literal_value;


namespace clumsy_lib
{
	
	export 
	template<auto v>
	struct literal_value
	{

		template<typename T>  
		static void assign(T& t)
		{
			t = v;
		}

	};

	export 
		template<typename T, T v >
	struct literal_value<v>
	{

		static void assign(T& t)
		{
			t = v;
		}

	};
}


