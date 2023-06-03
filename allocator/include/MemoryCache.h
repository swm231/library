#ifndef _Thread_Cache_H_
#define _Thread_Cache_H_

#include "Common.h"
#include "FreeList.h"
#include <memory>

class FreeList;
class MemoryCache{
private:
    // 8, 16, 32, 64, 128, 256, 512, 1k, 2k, 4k
    // 6, 10, 18, 34, 66, 130, 258, 514, 1k + 2, 2k + 2
    FreeList* freelist_[NLISTS];
public:
    void* Allocate(size_t);
    void Deallocate(void*);

    MemoryCache();
    ~MemoryCache();
};

template<typename T>
struct DsAllocator{
    template<typename... Types>
    void construct(T* p, Types&&... args){
        new (p) T(std::forward<Types>(args)...);
    }
    void destroy(T* p){
        p->~T();
    }
};

#endif