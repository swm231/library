#include "MemoryCache.h"

MemoryCache::MemoryCache(){
    for(int i = 0; i < 10; ++i){
        freelist_[i] = new (malloc(sizeof(FreeList))) FreeList(i);
    }
}
MemoryCache::~MemoryCache(){
    // TODO
}
void* MemoryCache::Allocate(size_t size){
    size_t index = Size::Index(size);
    return freelist_[index]->Get();
}
void MemoryCache::Deallocate(void* ptr, size_t size){
    size_t index = Size::Index(size);
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
void operator delete(void* ptr, size_t size){
    // TODO
}

void* operator new[](size_t size){
    return MC().Allocate(size);
}
void operator delete[](void* ptr){
    // TODO
}