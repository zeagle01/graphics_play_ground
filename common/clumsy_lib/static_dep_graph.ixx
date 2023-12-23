
module;


#include "clumsy_lib/static_loop.h"
#include "clumsy_lib/type_list.h"

#include <unordered_map>
#include <typeindex>
#include <set>
#include <functional> 

export module clumsy_lib : static_dep_graph;


namespace clumsy_lib
{

	template<typename T>
	struct default_get_deps_new
	{
		template<typename U>
		static auto  choosor()
		{
			if constexpr (requires {typename U::deps; })
			{
				return typename U::deps {};
			}
			else
			{
				return type_list<>{};
			}
		}

		using type = decltype(choosor<T>());
	};

	export
	class static_dep_graph
	{
	public:

		template<typename list, template<typename> typename get_deps = default_get_deps_new>
		void build();

		template<typename from, typename to>
		bool has_edge();

		const std::set<std::type_index>& get_outlets(std::type_index t) const;

		template<typename from>
		bool contains() const;

		bool contains(std::type_index t) const;

		using adj_list_t = std::unordered_map<std::type_index, std::set<std::type_index>>;

		const adj_list_t& get() const
		{
			return m_data;
		}


		static static_dep_graph merge(const static_dep_graph& dep0, const static_dep_graph& dep1);

	private:

		template<  template<typename> typename get_deps >
		struct collect_deps;

		adj_list_t m_data;

	};


	export
	class dynamic_walker
	{
	public:
		dynamic_walker(const static_dep_graph& _graph) :graph(_graph) {}

		void walk(std::type_index type, std::function<void(std::type_index)> fn);
		void walk(std::function<void(std::type_index)> fn);

	private:
		void walk_imp(std::type_index type, std::function<void(std::type_index)> fn);
		void walk_imp(std::function<void(std::type_index)> fn);

		const static_dep_graph& graph;
		std::set<std::type_index> visited;
	};

	template<typename T>
	struct always_update { static bool apply(...) { return true; } };

	export
	template<typename list, template<typename> typename get_deps>
	class static_walker
	{
	public:
		template<
				typename dispatchor, 
				template<typename> typename need_update_checker = always_update,
				typename ...P
		>
		void walk(P&& ...p);

	private:

		template< typename dispatchor, template<typename> typename need_update_checker>
		struct update_each
		{
			template<typename var_name, typename ...P>
			static void apply(static_walker& walker, P&& ...p)
			{
				recurse_update_upstream<var_name, dispatchor, need_update_checker, typename get_deps<var_name>::type>::apply(walker, std::forward<P>(p)...);
			}
		};

		template<typename var, typename dispatchor, template<typename> typename need_update_checker, typename dep_list>
		struct recurse_update_upstream;

		template<typename var, typename dispatchor, template<typename> typename need_update_checker, typename ...dep>
		struct recurse_update_upstream <var, dispatchor, need_update_checker, type_list< dep...>>
		{

			template<typename ... P>
			static void apply(static_walker& walker, P&& ...p)
			{
				if constexpr (Type_In_List<var, list>)
				{

					bool need_update = (check_need_update<dep, need_update_checker>(walker, std::forward<P>(p)...) || ...);

					if (need_update)
					{
						(apply_each<dep >::apply(walker, std::forward<P>(p)...), ...);
					}

					if (check_need_update<var, need_update_checker>(walker, std::forward<P>(p)...))
					{
						if constexpr (!(std::is_same_v<dispatchor, std::nullptr_t>))
						{
							dispatchor::template apply<var, dep...>(std::forward<P>(p)...);
						}
						walker.m_visited.insert(std::type_index(typeid(var)));
					}
				}

			}

			template<typename var, typename ... P>
			static void apply_each(static_walker& walker, P&& ...p)
			{
				//if constexpr (Type_In_List<var, list>)
				{
					recurse_update_upstream<var, dispatchor, need_update_checker, typename get_deps<var>::type>::apply(walker, std::forward<P>(p)...);
				}
			}

			template<typename var, template<typename> typename need_update_checker, typename ...P >
			static bool check_need_update(const static_walker& walker, P&& ...p)
			{
				return (!walker.m_visited.contains(std::type_index(typeid(var))))
					&& need_update_checker<var>::apply(std::forward<P>(p)...);
			}
		};
	private:
		std::set<std::type_index>  m_visited;
	};



	///////// static_dep_graph
	template<typename list, template<typename> typename get_deps >
	void static_dep_graph::build()
	{
		For_Each_Type<list>::apply<collect_deps< get_deps>>(m_data);
	}

	static_dep_graph static_dep_graph::merge(const static_dep_graph& dep0, const static_dep_graph& dep1)
	{

		static_dep_graph ret = dep0;
		for (const auto& var : dep1.m_data)
		{
			auto up = var.first;
			for (const auto& down : dep1.m_data.at(up))
			{
				if (ret.m_data.contains(up))
				{
					ret.m_data[up].insert(down);
				}
				else
				{
					ret.m_data[up] = std::set<std::type_index>{ down };
				}
			}

			if (!ret.m_data.contains(up))
			{
				ret.m_data[up] = std::set<std::type_index>{};
			}

		}
		return ret;
	}

	template< template<typename> typename get_deps >
	struct static_dep_graph::collect_deps
	{
		template<typename curr_t>
		struct add_down_stream
		{
			template<typename upstream_t>
			static void apply(adj_list_t& m_down_streams)
			{
				if (m_down_streams.contains(std::type_index(typeid(upstream_t))))
				{
					m_down_streams[std::type_index(typeid(upstream_t))].insert(std::type_index(typeid(curr_t)));
				}
				else
				{
					m_down_streams[std::type_index(typeid(upstream_t))] = std::set{ std::type_index(typeid(curr_t)) };
				}
			}
		};

		template<typename T>
		static void apply(adj_list_t& m_down_streams)
		{
			using deps = get_deps<T>::type;

			For_Each_Type<deps>::apply<add_down_stream<T>>(m_down_streams);
			if (!m_down_streams.contains(std::type_index(typeid(T))))
			{
				m_down_streams[std::type_index(typeid(T))] = std::set<std::type_index>{};
			}
		}

	};

	template<typename from, typename to>
	bool static_dep_graph::has_edge()
	{
		auto k_from = std::type_index(typeid(from));

		auto k_to = std::type_index(typeid(to));

		return m_data[k_from].contains(k_to);
	}

	const std::set<std::type_index>& static_dep_graph::get_outlets(std::type_index t) const
	{
		return m_data.at(t);
	}

	template<typename var>
	bool static_dep_graph::contains() const
	{
		auto key = std::type_index(typeid(var));
		return contains(key);

	}

	bool static_dep_graph::contains(std::type_index t) const
	{
		return m_data.contains(t);

	}


	/////////dynamic walker

	void dynamic_walker::walk(std::type_index type, std::function<void(std::type_index)> fn)
	{
		visited.clear();
		walk_imp( type,  fn);
	}

	void dynamic_walker::walk_imp(std::type_index type, std::function<void(std::type_index)> fn)
	{
		if ((!visited.contains(type)) && graph.contains(type))
		{
			visited.insert(type);

			fn(type);

			for (const auto& d : graph.get_outlets(type))
			{
				walk_imp(d,fn);
			}
		}
	}

	void dynamic_walker::walk(std::function<void(std::type_index)> fn)
	{
		visited.clear();
		walk_imp(fn);
	}

	void dynamic_walker::walk_imp(std::function<void(std::type_index)> fn)
	{
		const auto& graph_data = graph.get();
		for (const auto& it : graph_data)
		{
			walk_imp(it.first, fn);
		}
	}

	/////////static walker

	template<typename list, template<typename> typename get_deps>
	template<  typename dispatchor, template<typename> typename need_update_checker, typename ...P >
	void static_walker<list, get_deps>::walk(P&& ...p)
	{
		For_Each_Type<list>::template
			apply< 
			update_each< dispatchor, need_update_checker >
			>( 
				*this, std::forward<P>(p)...
			);

	}

}

