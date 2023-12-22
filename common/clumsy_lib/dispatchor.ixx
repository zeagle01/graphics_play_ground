module;

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>

export module clumsy_lib: dispatchor;


namespace clumsy_lib
{
	export
		template<typename base_type, typename return_type>
	class dispatchor
	{
	public:

		template<typename sub_type>
		using fn = std::function<return_type(const sub_type&)>;

		template<typename sub_type>
		void connect(fn<sub_type> sub_type_fn)
		{
			auto key = std::type_index(typeid(sub_type));

			m_funcs[key] = [sub_type_fn](const base_type& b)
				{
					return sub_type_fn(dynamic_cast<const sub_type&>(b));
				};
		}

		return_type operator()(const base_type& data)
		{
			auto key = std::type_index(typeid(data));
			if (m_funcs.contains(key))
			{
				return m_funcs[key](data);
			}

			return {};
		}

	private:
		using funcs_map = std::unordered_map<std::type_index, fn<base_type>>;
		funcs_map m_funcs;
	};
}
