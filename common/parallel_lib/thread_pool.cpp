
module;

#include <functional>
#include <thread>
#include <mutex>

module parallel_lib: thread_pool;


namespace parallel
{
	class thread_pool_imp
	{
	public:
		thread_pool_imp(int thread_num):m_thread_num(thread_num)
		{
			m_want_stop = false;
			m_is_job_done = true;

			if (m_debug)
			{
				printf(" start \n");
			}

			start_threads();
		}

		~thread_pool_imp()
		{
			stop_and_join_threads();
		}


		void wait_until_completed()
		{

			std::unique_lock lock(m_lock);

			m_cv1.wait(lock,
				[this]()
				{
					if (m_debug)
					{
						printf(" check job is %d ! \n", int(m_is_job_done));
					}

					return  m_is_job_done;
				}
			);

		}

		void add_task(std::function<void()> task)
		{
			std::lock_guard lock(m_lock);
			m_tasks.push_back(task);
			m_cv.notify_one();
			m_is_job_done = false;

		}

	private:
		void start_threads()
		{
			for (int i = 0; i < m_thread_num; i++)
			{
				m_threads.emplace_back(
					[this]() {

						while (true)
						{


							std::function<void()> fn;
							int taskNum = 0;

							{
								std::unique_lock lock(m_lock);


								m_cv.wait(lock,
									[this]()
									{
										return m_want_stop || !m_tasks.empty();
									}
								);

								taskNum = m_tasks.size();

								if (m_want_stop && m_tasks.empty())
								{
									break;
								}

								if (!m_tasks.empty())
								{
									fn = m_tasks.back();
									m_tasks.pop_back();
								}
							}


							if (fn)
							{
								if (m_debug)
								{
									printf(" %d doing job! %d\n", std::this_thread::get_id(), taskNum);
								}
								fn();
							}

							if (taskNum == 0 || taskNum == 1)
							{
								if (m_debug)
								{
									printf("%d do the last job \n", std::this_thread::get_id());
								}

								m_is_job_done = true;
								m_cv1.notify_one();
							}

						}
					});
			}

		}

		void stop_and_join_threads()
		{
			{
				std::lock_guard lock(m_lock);
				m_want_stop = true;
			}
			m_cv.notify_all();

			for (auto& t : m_threads)
			{
				t.join();
			}
		}

	private:
		int m_thread_num;
		std::vector<std::function<void()>> m_tasks;
		std::vector<std::thread> m_threads;
		static constexpr bool m_debug = false;
	private:
		std::condition_variable m_cv;
		std::condition_variable m_cv1;
		std::mutex m_lock;
		bool m_want_stop;
		bool m_is_job_done;

	};

}

