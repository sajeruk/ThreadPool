#include "thread_pool.h"

namespace pool {
    ThreadPool::Worker::Worker(ThreadPool& pool)
        : Pool(pool)
    {
        Pool.WaitsTask.notify_one();
    }

    void ThreadPool::Worker::operator()() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(Pool.Mutex);
                Pool.HasTask.wait(lock, [this]() {
                    return !Pool.Working || !Pool.Tasks.empty();
                });
                if (!Pool.Working && Pool.Tasks.empty()) {
                    return;
                }
                task = std::move(Pool.Tasks.front());
                Pool.Tasks.pop();
            }
            task();
            Pool.WaitsTask.notify_one();
        }
    }

    ThreadPool::ThreadPool(size_t poolSize)
            : Working(true)
    {
        Workers.reserve(poolSize);
        for (size_t i = 0; i < poolSize; ++i) {
            Workers.emplace_back(Worker(*this));
        }
        ManageThread = std::thread(&ThreadPool::ManageTasks, this);
    }

    void ThreadPool::ManageTasks() {
        while (true)
        {
            std::unique_lock<std::mutex> lock(Mutex);
            WaitsTask.wait(lock, [this]() {
                return !Working || !Tasks.empty();
            });
            if (!Working) {
                return;
            } else if (!Tasks.empty()) {
                HasTask.notify_one();
            }
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(Mutex);
            Working = false;
        }
        HasTask.notify_all();
        WaitsTask.notify_all();
        for (auto& worker : Workers) {
            worker.join();
        }
        ManageThread.join();
    }

}