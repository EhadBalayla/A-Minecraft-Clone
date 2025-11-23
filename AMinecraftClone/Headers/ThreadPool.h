#pragma once
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>

class Chunk;
class WorldManager;
struct job {
	void(*ptr)(Chunk*, WorldManager*);
	Chunk* c;
	WorldManager* world;
};
class ThreadPool
{
public:
	ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
	~ThreadPool();

	void QueueJob(job j);
private:
	void ThreadLoop();

	bool running = true;
	std::mutex queue_mtx;
	std::vector<std::thread> threads;
	std::queue<job> jobs_queue;
	std::condition_variable cv;
};