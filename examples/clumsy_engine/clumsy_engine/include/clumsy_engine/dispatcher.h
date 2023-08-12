
#pragma once


#include <map>
#include <functional>



namespace clumsy_engine
{


	template<typename  Base_T, typename Ret  >
	class Dispatcher
	{

		using key_t = std::string;
		using fn_t = std::function < Ret(Base_T*)>;
		std::map < key_t, fn_t> m_function_map;

	public:

		~Dispatcher() { }

		template<typename Sub_T>
		void add(std::function < Ret(Sub_T& )> fn)
		{
			//add static assert that Sub_T is subclass of Base_T
			fn_t auto_cast_wrapper = [fn](Base_T* s)
			{
				return fn(*dynamic_cast<Sub_T*>(s));
			};

			key_t key = typeid(Sub_T).name();

			m_function_map[key] = auto_cast_wrapper;
		}


		Ret operator()(Base_T& b)
		{
			key_t key = typeid(b).name();
			if (m_function_map.count(key))
			{
				return m_function_map[key](&b);
			}
			else
			{
				return Ret{};
			}

		}
	};


}