#pragma once
#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <exception>
#include <future>
#include <mutex>
#include <queue>

namespace MGE {

    class ThreadPool
    {
    public:
        void Start();
        void QueueJob(const std::function<void()>& job);
        void Stop();
        bool busy();
        void pause();
        void resume();
    
    private:
        void ThreadLoop();
    
        bool m_pause = false;
        bool should_terminate = false;           // Tells threads to stop looking for jobs
        std::mutex queue_mutex;                  // Prevents data races to the job queue
        std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> jobs;
    };

}
