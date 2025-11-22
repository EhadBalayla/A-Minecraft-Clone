#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class SafeQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable condVar;

public:
    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(item);
        }
        condVar.notify_one();
    }

    // This blocks until an item is available
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex);
        condVar.wait(lock, [this]() { return !queue.empty(); });

        item = queue.front();
        queue.pop();
        return true;
    }

    // This tries to pop but doesn't block (used in main thread)
    bool try_pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) return false;

        item = queue.front();
        queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }
};
