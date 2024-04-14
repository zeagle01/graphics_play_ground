
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

}
