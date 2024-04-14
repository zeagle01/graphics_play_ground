
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
		thread_pool_imp(int thead_num)
		{
			m_want_stop = false;


			for (int i = 0; i < thead_num; i++)
			{
				m_threads.emplace_back(
					[this]() {

						while (true)
						{

							std::function<void()> fn;

							{
								std::unique_lock lock(m_lock);


								m_cv.wait(lock,
									[this ]()
									{
										return m_want_stop || !m_tasks.empty();
									}
								);

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
								fn();
							}

						}
					});
			}
		}

		~thread_pool_imp()
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

		void add_task(std::function<void()> task)
		{
			std::lock_guard lock(m_lock);
			m_tasks.push_back(task);
			m_cv.notify_one();

		}

	private:
		std::vector<std::function<void()>> m_tasks;
		std::vector<std::thread> m_threads;
	private:
		std::condition_variable m_cv;
		std::mutex m_lock;
		bool m_want_stop;

	};

}

