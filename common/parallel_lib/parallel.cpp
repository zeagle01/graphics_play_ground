
module;

#include <memory>

module parallel_lib;




namespace parallel
{

	thread_pool::thread_pool(int thread_num)
	{
		m_imp = std::make_shared<thread_pool_imp>(thread_num);
	}


	void thread_pool::add_task(std::function<void()> task)
	{
		m_imp->add_task(task);
	}


	void thread_pool::wait_until_completed()
	{
		m_imp->wait_until_completed();
	}


	void for_each(int size, int grain, std::function<void(int)> fn)
	{
		static thread_pool l_thread_pool(24);

		int group = (size + grain - 1) / grain;

		for (int gi = 0; gi < group; gi++)
		{
			l_thread_pool.add_task(
				[gi,size,grain,fn]() {
					for (int ti = 0; ti < grain; ti++)
					{
						int i = gi * grain + ti;
						if (i < size)
						{
							fn(i);

						}
					}
				}
			);
		}

		l_thread_pool.wait_until_completed();

	}

}
