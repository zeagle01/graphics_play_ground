
#pragma once

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
//	struct Broad_Cast_Data
//	{
//		using cr_signal = signal<const T&>;
//		using cr_slot = slot<const T&>;
//
//		T data;
//		cr_signal sig;
//
//		void send_to(cr_slot s)
//		{
//			sig.connect(s);
//		}
//
//		void set_and_broad_cast(const T& data)
//		{
//			this->data = data;
//
//			sig(data);
//		}
//	};

}

