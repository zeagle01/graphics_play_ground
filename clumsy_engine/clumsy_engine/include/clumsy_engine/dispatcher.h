
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

		~Dispatcher() {

			printf("in \n");

		}

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

//		template<typename poly_base_t>
//		Ret invoke(poly_base_t&& b)
//		{
//			using std::forward<poly_base_t>(b);
//			static_assert(std::is_same_v<std::decay<poly_base_t>, Base_T>);
//
//			key_t key = typeid(b).name();
//
//			if constexpr (std::is_reference_v<poly_base_t>)
//			{
//				return m_function_map[key](&b);
//			}
//			else if constexpr (std::is_pointer_v<poly_base_t>)
//			{
//				return m_function_map[key](b);
//			}
//			else
//			{
//				static_assert(0);
//				return R{};
//			}
//
//		}
//
//		template<typename Sub_T>
//		void add(std::function < Ret(Sub_T)> fn)
//		{
//			fn_t auto_cast_wrapper; 
//
//			if constexpr (std::is_reference_v<Sub_T>)
//			{
//				auto_cast_wrapper = [fn](Base_T* s)
//				{
//					return fn(*dynamic_cast<Sub_T*>(s));
//				};
//			}
//			else if constexpr (std::is_pointer_v<Sub_T>)
//			{
//				auto_cast_wrapper = [fn](Base_T* s)
//				{
//					return fn(dynamic_cast<Sub_T*>(s));
//				};
//			}
//			else
//			{
//				static_assert(0);
//			}
//
//			key_t key = typeid(std::decay_t<Sub_T>).name();
//
//			m_function_map[key] = auto_cast_wrapper;
//		}

	};


}