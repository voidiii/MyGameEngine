#include "MGEpch.h"
#include "ThreadPool.h"

void MGE::ThreadPool::Start()
{
    const uint32_t num_threads = std::thread::hardware_concurrency(); // Max # of threads the system supports
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void MGE::ThreadPool::QueueJob(const std::function<void()>& job)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

void MGE::ThreadPool::Stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

bool MGE::ThreadPool::busy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void MGE::ThreadPool::ThreadLoop()
{
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
                });
            if (should_terminate) {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}
