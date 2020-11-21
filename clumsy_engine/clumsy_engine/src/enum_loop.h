



namespace clumsy_engine
{
	template<typename E, E V>
	constexpr bool is_valid()
	{
		auto name = __FUNCSIG__;
		int i = 0;
		for (;; i++)
		{
			if (name[i] == '>')
			{
				i--;
				break;
			}
			if (name[i] == '\0')
			{
				return false;
			}

		}


		for (; i >= 0; i--)
		{
			if (name[i] == ')') {
				break;
			}
		}
		if (i == -1)
		{
			return true;
		}

		if (name[i + 1] == '0' && name[i + 2] == 'x')
		{
			return false;
		}

		return true;
	};




	template<typename E, int h = 0 >
	struct loop_enum
	{
		template<template<typename U, U u>  typename F, typename ...P>
		static void apply(P&&... p)
		{
			if constexpr (is_valid<E, E(h)>())
			{
				F<E, E(h)>::apply(std::forward<P>(p)...);
				loop_enum<E, h + 1>::template apply<F>(std::forward<P>(p)...);
			}
		}

	};

}


