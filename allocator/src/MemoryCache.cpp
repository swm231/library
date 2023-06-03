#include "MemoryCache.h"

MemoryCache::MemoryCache(){
    for(int i = 0; i < 10; ++i)
        freelist_[i] = new (malloc(sizeof(FreeList))) FreeList(i);
}
MemoryCache::~MemoryCache(){
    for(int i = 0; i < 10; ++i)
        freelist_[i]->~FreeList();
}
void* MemoryCache::Allocate(size_t size){
    size += 2;
    size_t index = Size::Index(size);
    short* ptr = (short*)freelist_[index]->Get();
    *ptr = (short)Size::Roundup(size);
    ptr = (short*)((uint64_t)ptr + 2);
    return ptr;
}
void MemoryCache::Deallocate(void* ptr){
    ptr = (void*)((uint64_t)ptr - 2);
    size_t index = Size::Index(*(short*)ptr);
    freelist_[index]->Push(ptr);
}

SpanList* CentralCache::spanlist_ = new (malloc(sizeof(SpanList))) SpanList();

MemoryCache& MC(){
    static MemoryCache MC;
    return MC;
}

void* operator new(size_t size){
    return MC().Allocate(size);
}
void operator delete(void* ptr){
    MC().Deallocate(ptr);
}

void* operator new[](size_t size){
    return MC().Allocate(size);
}
void operator delete[](void* ptr){
    MC().Deallocate(ptr);
}