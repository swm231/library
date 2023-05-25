#ifndef _Thread_Cache_H_
#define _Thread_Cache_H_

#include "Common.h"
#include "FreeList.h"
#include <memory>

class FreeList;
class MP{
private:
    // 8, 16, 32, 64, 128, 256, 512, 1k, 4k, 8k
    FreeList* freelist_[NLISTS];
public:
    void* Allocate(size_t);
    void Deallocate(void*, size_t);

    MP();
    ~MP();

    void* operator new(size_t size){

    }
    void  operator delete(void* ptr){

    }
};

#endif