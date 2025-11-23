#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) {
	for (size_t i = 0; i < numThreads; i++) {
		threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
	}
}
ThreadPool::~ThreadPool() {
	running = false;

	cv.notify_all();

	for (size_t i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}
void ThreadPool::QueueJob(job j) {
	{
		std::lock_guard<std::mutex> lock(queue_mtx);
		jobs_queue.push(j);
	}
	cv.notify_one();
}


void ThreadPool::ThreadLoop() {
	while (running) {
		job j;
		{
			std::unique_lock<std::mutex> lock(queue_mtx);
			cv.wait(lock, [this] { return !jobs_queue.empty() || !running; });

			if (!running) { return; }

			j = jobs_queue.front();
			jobs_queue.pop();
		}
		j.ptr(j.c, j.world);
	}
}