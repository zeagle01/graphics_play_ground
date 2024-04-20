
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

	void thread_pool::start()
	{
		m_imp->start();
	}

	void thread_pool::wait_until_completed()
	{
		m_imp->wait_until_completed();
	}

}
