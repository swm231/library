#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <exception>
#include <cstdio>
#include <functional>
#include "MemoryCache.h"

MemoryCache& MC();
template<typename T>
struct DsAllocator;

template<typename T, typename Alloc = DsAllocator<T>>
class vector{
public:
    typedef T value_type;
    typedef T* iterator;
private:
    value_type* data_;  // 地址指针
    size_t size_;       // 元素个数
    size_t capacity_;   // 申请空间的大小
    Alloc allocator_;

    void _expand(){
        if(capacity_ == 0){
            data_ = static_cast<value_type*>(MC().Allocate(sizeof(value_type)));
            capacity_ = 1;
        }
        else {
            capacity_ *= 2;
            value_type* temp = static_cast<value_type*>(MC().Allocate(sizeof(value_type) * capacity_));
            for(int i = 0; i < size_; ++i)
                allocator_.construct(temp + sizeof(T) * i, std::move(*(data_ + sizeof(T) * i)));
            delete data_;
            data_ = temp;
        }
    }
    template<typename... Types>
    void _push_back(value_type* _pos, Types&&... args){
        allocator_.construct(_pos, std::forward<Types>(args)...);
    }
    void _pop_back(){
        allocator_.destroy(data_ + sizeof(T) * size_);
    }
public:
    vector(): data_(nullptr), size_(0), capacity_(0){}
    vector(size_t size): data_(new value_type[size]), size_(size), capacity_(size) {}
    ~vector(){
        for(uint32_t i = 0; i < size_; ++i)
            allocator_.destroy(data_ + sizeof(T) * i);
        MC().Deallocate(data_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    vector(const vector& vec){
        size_ = vec.size_;
        capacity_ = vec.capacity_;
        data_ = new value_type[capacity_];
        for(int i = 0; i < size_; ++i)
            data_[i] = vec.data_[i];
    }

    vector operator= (const vector& vec){
        if(this == &vec) return *this;
        delete [] data_;
        data_ = new value_type(vec.capacity_);
        size_ = vec.size_;
        capacity_ = vec.capacity_;
        for(int i = 0; i < size_; ++i)
            data_[i] = vec.data_[i];
        return *this;
    }
    value_type& operator[] (size_t index){
        return *(data_ + sizeof(T) * index);
    }

    bool operator== (const vector& vec) const{
        if(size_ != vec.size_) return false;
        for(int i = 0; i < size_; ++i)
            if(data_[i] != vec.data_[i])
                return false;
        return true;
    }

    template<typename... Types>
    void emplace_back(Types&&... args){
        if(size_ == capacity_)
            _expand();
        _push_back(data_ + sizeof(T) * size_, std::forward<Types>(args)...);
        ++ size_;
    }
    void push_back(const value_type& value){
        if(size_ == capacity_)
            _expand();
        _push_back(data_ + sizeof(T) * size_, value);
        ++ size_;
    }

    void check() const{
        if(size_ == 0)
            throw std::runtime_error("vector is empty!");
    }
    void pop_back(){
        check();
        _pop_back();
        -- size_;
    }
    void clear(){
        size_ = 0;
    }
    bool empty(){
        return size_ == 0;
    }
    int size() const{
        return size_;
    }
    void swap(vector& vec){
        swap(data_, vec.data_);
        swap(size_, vec.size_);
        swap(capacity_, vec.capacity_);
    }
    value_type& front()const{
        check();
        return data_[0];
    }
    value_type& back()const{
        check();
        return data_[size_ - 1];
    }
    iterator begin() const{
        check();
        return data_;
    }
    iterator end() const{
        check();
        return data_ + size_;
    }
    size_t capacity() const{
        return capacity_;
    }
};

#endif