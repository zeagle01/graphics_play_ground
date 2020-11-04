
#pragma once
#include <functional>

namespace clumsy_engine
{

	template<typename ...P>
	using slot = std::function<void(P...)>;

	template<typename ...P>
	struct signal
	{
		void connect(slot<P...> s)
		{
			slots.push_back(s);
		}

		void operator()(P&&... p)
		{
			for (auto& s : slots)
			{
				s(std::forward<P>(p)...);
			}
		}

		std::vector<slot<P...>> slots;
	};

//	template<typename T>
//	using sennder = std::function<const T & ()>;
//
//	template<typename T>
//	struct receiver
//	{
//		void connect(sender<T> s)
//		{
//			slots.push_back(s);
//		}
//
//		void operator()(P&&... p)
//		{
//			for (auto& s : slots)
//			{
//				s(std::forward<P>(p)...);
//			}
//		}
//
//		std::vector<sender<T>> senders;
//	};

}

