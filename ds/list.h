#ifndef _LIST_H_
#define _LIST_H_ 1

#include <exception>
#include <cstdio>
#include <functional>
// #include "/home/swm_231/library/allocator/include/MemoryCache.h"

template<typename T>
class _list_iterator;

template<typename T>
class _list_node{
public:
    typedef _list_node<T> _self;
    typedef T value_type;
    typedef _list_iterator<T> _iterator;
    T _value;
    _self* _pre;
    _self* _nxt;

    explicit _list_node(): _pre(nullptr), _nxt(nullptr) {}
    explicit _list_node(_self& _val): _value(_val._value), _pre(_val._pre), _nxt(_val._nxt){} //
    explicit _list_node(const value_type& value_) : _value(value_), _pre(nullptr), _nxt(nullptr){}
    template<typename... Types>
    explicit _list_node(Types&&... args) : _value(std::forward<Types>(args)...), _pre(nullptr), _nxt(nullptr){}
    ~_list_node() {}

    void _hook(_self* const _pos){
        _pre = _pos->_pre;
        _nxt = _pos;
        (_pos->_pre)->_nxt = this;
        _pos->_pre = this;
    }
    void _unhook(){
        _pre->_nxt = _nxt;
        _nxt->_pre = _pre;
    }

    T& Value(){ return _value; }
};

template<typename T>    
class _list_iterator{
public:
    typedef _list_node<T>       _list_node_base;
    typedef _list_iterator<T>   _self;
    typedef T                   _value_type;
    typedef T*                  pointer;
    typedef T&                  reference;

    explicit _list_iterator(): _node(new _list_node_base()) {}
    explicit _list_iterator(_list_node_base* _p) : _node(_p) {}
    explicit _list_iterator(const T& value_) : _node(new _list_node_base(value_)) {}
    template<typename... Types>
    explicit _list_iterator(Types&&... args): _node(new _list_node_base(std::forward<Types>(args)...)) {}
    ~_list_iterator(){ }

    _self operator = (const _self& val){ *this = val; return *this; }

    friend bool operator == (const _self& _x, const _self& _y)
    { return _x._node == _y._node; }
    friend bool operator != (const _self& _x, const _self& _y)
    { return !(_x._node == _y._node); }
    reference operator* (){ return _node->Value(); }
    _list_node_base* operator-> (){ return _node;}

    _self& operator ++ (){ 
        _node = _node->_nxt;
        return *this;
    }
    _self operator ++ (int){
        _self _tmp = *this;
        _node = _node->_nxt;
        return _tmp;
    }
    _self& operator -- (){ 
        _node = _node->_pre;
        return *this;
    }
    _self operator -- (int){
        _self _tmp = *this;
        _node = _node->_pre;
        return _tmp;
    }
    _list_node_base* _node; 
};


// MemoryCache& MC();
template<typename T>
struct DsAllocator;

// template<typename T, typename Alloc = DsAllocator<_list_iterator<T>>>
template<typename T>
class list{
public: 
    typedef T                   value_type;
    typedef T*                  pointer;
    typedef _list_iterator<T>   iterator;
    typedef _list_node<T>       _list_node_base;
private:
    int _size;
    iterator* _impl;
    // Alloc allocator_;

    template<typename... Types>
    void _insert(iterator _pos, Types&&... args){
        _list_node_base* _tmp = new _list_node_base(std::forward<Types>(args)...);
        _tmp->_hook(_pos._node);
        ++ _size;
    }
    void _erase(iterator _pos){
        if(_size == 0 || _pos == end())
            return;
        _pos._node->_unhook();
        -- _size;
        delete _pos._node;
    }

    _list_node_base* _sort_list(_list_node_base* head){
        if(head == end()._node || head->_nxt == end()._node) return head;
        _list_node_base *slow = head, *fast = head;
        while(fast->_nxt != end()._node && fast->_nxt->_nxt != end()._node){
            fast = fast->_nxt->_nxt;
            slow = slow->_nxt;
        }
        fast = slow;
        slow = slow->_nxt;
        fast->_nxt = end()._node;
        fast = _sort_list(head);
        slow = _sort_list(slow);
        return _merge(fast, slow);
    }
    _list_node_base* _merge(_list_node_base* _x, _list_node_base* _y){
        if(_x == end()._node) return _y;
        if(_y == end()._node) return _x;
        _list_node_base *_begin, *_end;
        if(_x->Value() < _y->Value())
            _begin = _end = _x, _x = _x->_nxt;
        else 
            _begin = _end = _y, _y = _y->_nxt;
        
        while(_x != end()._node && _y != end()._node){
            if(_x->Value() < _y->Value())
                _end->_nxt = _x,
                _x = _x->_nxt;
            else
                _end->_nxt = _y,
                _y = _y->_nxt;
            _end = _end->_nxt;
        }
        if(_x != end()._node) _end->_nxt = _x;
        else if(_y != end()._node) _end->_nxt = _y;
        return _begin;
    }

public:
    list() : _size(0), _impl(new iterator) {
        (*_impl)->_nxt = _impl->_node;
        (*_impl)->_pre = _impl->_node;
    }
    ~list() {
        iterator it = begin();
        while(it != end()){
            _list_node_base* _tmp = it._node;
            ++it;
            delete _tmp;
        }
        delete (*_impl)._node;
        delete _impl;
    }

    // const iterator* crbegin(){ return ; }
    // const iterator* crend(){ return static_cast<const iterator*>(_end); }

    // iterator* rbegin(){ return static_cast<iterator*>(_begin); }
    // const iterator* rend(){ return static_cast<iterator*>(_end); }

    const iterator cbegin(){ return iterator(_impl->_node->_nxt); }
    const iterator cend(){ return iterator(_impl->_node); }

    iterator begin(){ return iterator(_impl->_node->_nxt); }
    const iterator end(){ return iterator(_impl->_node); }


    bool empty(){ return _size == 0; }
    size_t size(){ return _size; }

    T& front(){ return static_cast<T&>(_impl->_node->_nxt->Value()); }
    T& back() { return static_cast<T&>(_impl->_node->Value()); }

    template<typename... Types>
    void emplace_front(Types&&... args){
        _insert(begin(), std::forward<Types>(args)...);
    }
    void push_front(const value_type& value){
        _insert(begin(), value);
    }
    void pop_front(){
        if(_size == 0)
            return;
        _erase(begin());
    }

    template<typename... Types>
    void emplace_back(Types&&... args){
        _insert(end(), std::forward<Types>(args)...);
    }
    void push_back(const value_type& value){
        _insert(end(), value);
    }
    void pop_back(){
        if(_size == 0)
            return;
        _erase(iterator((*_impl)->_pre));
    }

    template<typename... Types>
    void emplace(iterator _pos, Types&&... args){
        _insert(_pos, std::forward<Types>(args)...);
    }
    void insert(iterator _pos, iterator _it){
        _insert(_pos, _it);
    }

    void erase(iterator _it){
        _erase(_it); 
    }
    void swap(_list_node_base _x, _list_node_base _y){
        swap(_x._nxt, _y._nxt);
        swap(_x._pre, _y._pre);
    }
    void clear(){
        iterator it = begin();
        while(it != end()){
            _list_node_base* _tmp = it._node;
            ++it;
            delete _tmp;
        }
        _size = 0;
    }
    void remove(const T& _val){
        iterator it = begin();
        while(it != end()){
            if(it->Value() == _val){
                _list_node_base* _tmp = it._node;
                ++ it;
                _tmp->_unhook();
                delete _tmp;
                _size --;
            }
            else ++ it;
        }
    }
    void sort(){ _impl->_node->_nxt = _sort_list(begin()._node); }
};

#endif