#include "Thread_pool.h"

thread_pool::thread_pool(int _n) : stop(false){
    for(int i = 0; i < _n; ++ i)
        workers_.emplace_back([this](){
            for(;;){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]{ return stop || !task_que_.empty(); });
                    if(stop && task_que_.empty())
                        return;
                    task = std::move(task_que_.front());
                    task_que_.pop();
                }
                task();
            }
        });
}

thread_pool::~thread_pool(){
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for(auto& worker : workers_)
        worker.join();
}