#include "ThreadPool.h"

ThreadPool::ThreadPool(int threads) : stop(false)
{
	for (int i = 0; i < threads; i++)
		workers.push_back(std::thread(
			[this]{
					while(true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->queue_mutex);
							this->condition.wait(lock,
								[this] { return this->stop || !this->tasks.empty(); });
							if (this->stop && this->tasks.empty())
							{
								return;
							}
							task = std::move(this->tasks.front());
							this->tasks.pop();
						}

						task();
					}
				}
		));
}

void ThreadPool::enqueue(std::function<void()> f)
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		tasks.emplace(f);
	}
	condition.notify_one();
}

void ThreadPool::finishCurentTasksAndStop()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}

	condition.notify_all();
	for (std::thread &worker : workers) {
		worker.join();
	}
}


