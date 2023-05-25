#include "MemoryCache.h"

// bool MemoryCache::MemoryCache_Ready = false;
MP::MP(){
    printf("!@#\n");
    for(int i = 0; i < 10; ++i)
        freelist_[i] = new FreeList(i);
    // MemoryCache_Ready = true;
}
MP::~MP(){
    // TODO
}
void* MP::Allocate(size_t size){
    size_t index = Size::Index(size);
    return freelist_[index]->Get();
}
void MP::Deallocate(void* ptr, size_t size){
    size_t index = Size::Index(size);
    freelist_[index]->Push(ptr);
}

SpanList* CentralCache::spanlist_ = new SpanList();

void* MP::operator new(size_t size){

}