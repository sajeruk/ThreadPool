#include "../thread_pool.h"
#include <iostream>

int main() {
    using pool::ThreadPool;
    auto beg = std::chrono::high_resolution_clock::now();
    {
        ThreadPool threadPool(3000);
        for (size_t i = 0; i < 200000; ++i) {
            threadPool.AddTask([i]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            });
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "ThreadPool time: " <<
        std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count() <<
        " ms." << std::endl;

    beg = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    threads.reserve(200000);
    for (size_t i = 0; i < 200000; ++i) {
        threads.emplace_back([i]() {
             std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });
    }
    for (auto& thr : threads) {
        thr.join();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "default time: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count() <<
    " ms." << std::endl;

    return 0;
}