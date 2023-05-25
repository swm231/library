#ifndef _Common_H_
#define _Common_H_

#include <cstddef>
#include <cstdio>
#include <mutex>
#include <atomic>
#include <algorithm>

#define NLISTS 10

inline static void* & NxtObj(void* obj)
{
    return *(void**) obj;
}

class Size{
private:
    static size_t _Index(size_t size)
    {
        return std::max(32 - __builtin_clz(size) - 3 - (__builtin_popcount(size) == 1), 0);
    }
    static size_t _Roundup(size_t size)
    {
        return 1 << std::max(32 - __builtin_clz(size) - (__builtin_popcount(size) == 1), 3);
    }
public:
    static size_t Index(size_t size)
    {
        return _Index(size);
    }
    static size_t Roundup(size_t size)
    {
        return _Roundup(size);
    }
};

struct Span{
    void* ptr_;
    void* nxt_;
    Span(){
        ptr_ = malloc(8 * 1024);
        nxt_ = nullptr;
    }
};

class SpanList{
private:
    Span* head_;
    std::mutex mtx_;
public:
    SpanList(): head_(nullptr) {}
    ~SpanList() {
        Span* cur = head_;
        while(cur != head_->nxt_)
        {
            head_ = (Span*)head_->nxt_;
            delete cur;
            cur = head_;
        }
        delete head_;
        head_ = nullptr;
    }
    SpanList(const SpanList*) = delete;
    SpanList& operator= (const SpanList&) = delete;

    void AddSpan()
    {
        Span* obj = new (malloc(sizeof(Span))) Span();
        std::unique_lock<std::mutex> lock(mtx_);
        
        obj->nxt_ = head_;
        head_ = obj;
    }

    Span* GetSpan()
    {
        std::unique_lock<std::mutex> lock(mtx_);

        Span* obj = head_;
        head_ = (Span*)head_->nxt_;
        obj->nxt_ = obj;
        return obj;
    }

    bool Empty()
    {
        std::unique_lock<std::mutex> lock(mtx_);
        return head_ == nullptr;
    }
};

#endif