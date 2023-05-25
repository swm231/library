#ifndef _THREAD_POOP_H_
#define _THREAD_POOP_H_

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>

class thread_pool{
public:
    explicit thread_pool(int);
    ~thread_pool();

    template<class F, class... Args>
    auto add_task(F&& f, Args&&... args) ->std::future<decltype(f(args...))>;
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()> > task_que_;
    std::condition_variable cv;
    std::mutex mtx;
    bool stop;
};

template<class F, class... Args>
auto thread_pool::add_task(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
{
    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...) );
    {
        std::unique_lock<std::mutex> lock(mtx);
        if(stop)
            throw std::runtime_error("thread_pool has stop");
        task_que_.push([task_ptr]{ (*task_ptr)(); });
    }
    cv.notify_one();
    return task_ptr->get_future();
}

#endif