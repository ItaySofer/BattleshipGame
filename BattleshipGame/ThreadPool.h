#ifndef ThreadPoolH
#define ThreadPoolH

#include <vector>
#include <queue>
//#include <memory>
#include <thread>
#include <mutex>
//#include <condition_variable>
//#include <future>
#include <functional>
//#include <stdexcept>
//#include "MatchResult.h"

class ThreadPool {
public:
	ThreadPool(int threads);
	void enqueue(std::function<void()> f);
	void finishCurentTasksAndStop();
	~ThreadPool();
private:
	std::vector< std::thread > workers;
	std::queue< std::function<void()> > tasks;

	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;
};

#endif