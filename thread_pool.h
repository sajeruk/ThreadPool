#pragma once
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <memory>

namespace pool {
    class ThreadPool {
    private:
        class Worker {
        public:
            Worker(ThreadPool& pool);
            void operator()();

        private:
            ThreadPool& Pool;
        };
    public:
        ThreadPool(size_t poolSize);

        template<class Func, class... Args>
        auto AddTask(Func&& func, Args&&... args)
                -> std::future<decltype(func(args...))>
        {
            using result_type = decltype(func(args...));
            auto task = std::make_shared<std::packaged_task<result_type()>>(
                    std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );
            auto result = task->get_future();
            {
                std::unique_lock<std::mutex> lock(Mutex);
                if (!Working) {
                    throw std::runtime_error("Cannot AddTask after stopping ThreadPool");
                }
                Tasks.emplace([task]() { (*task)(); });
            }
            return result;
        }

        ~ThreadPool();

    private:
        void ManageTasks();

    private:
        std::vector<std::thread> Workers;
        std::queue<std::function<void()>> Tasks;
        std::condition_variable WaitsTask;
        std::condition_variable HasTask;
        std::mutex Mutex;
        std::thread ManageThread;
        bool Working;
    };

}