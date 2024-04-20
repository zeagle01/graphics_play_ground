
module;

// include things here
#include <functional>
#include <memory>

export module parallel_lib;

import : thread_pool;

namespace parallel
{
	export void for_each(std::function<void(int)> fn, int size, int grain);


	//class thread_pool_imp;
	export class thread_pool
	{
	public:
		thread_pool(int thread_num);

		void start();

		void add_task(std::function<void()> fn);

		void wait_until_completed();

	private:
		std::shared_ptr<thread_pool_imp> m_imp;
	};
}
