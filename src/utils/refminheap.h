//
// Created by mushan
//

#ifndef MYSERVER_REFMINHEAP_H
#define MYSERVER_REFMINHEAP_H

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <stddef.h>

typedef std::unordered_map<int ,int> Map;

template<class T>
class RefMinHeap {
private:
    std::unordered_map<size_t, size_t> ref_;
    std::vector<T> heap_;
    size_t capacity_;

public:
    RefMinHeap(int capacity);
    ~RefMinHeap();

    void push(size_t ref, T t);
    void pop();
    void modify(size_t ref, T t);
    T& refGet(size_t ref);

    T top() const;

    int size() {return heap_.size();}


private:
    void swim_(size_t index);
    bool sink_(size_t index, size_t n);
    void swap_(size_t index, size_t j);
    void delete_(size_t index);


};


template <class T>
RefMinHeap<T>::RefMinHeap(int capacity)
        :capacity_(capacity){
    heap_.reserve(capacity_);
}

template <class T>
RefMinHeap<T>::~RefMinHeap() = default;


template <class T>
T &RefMinHeap<T>::refGet(size_t ref) {
    return heap_[ref_[ref]];
}


template <class T>
void RefMinHeap<T>::swap_(size_t i, size_t j) {
    std::swap(heap_[i], heap_[j]);
    ref_[heap_[i].get_ref()] = i;
    ref_[heap_[i].get_ref()] = j;
}



template <class T>
void RefMinHeap<T>::swim_(size_t i) {
    size_t j = (i - 1) / 2;
    while (j >= 0 && heap_[i] < heap_[j]) {
        swap_(i, j);
        i = j;
        j = (i - 1) / 2;
    }
}


template <class T>
bool RefMinHeap<T>::sink_(size_t k, size_t n) {
    size_t i = k;
    size_t j = k * 2 + 1;
    while (j < n) {
        if (j + 1 < n && heap_[j+1] < heap_[j]) j++;
        if (heap_[i] < heap_[j]) break;
        swap_(i, j);
        i = j;
        j = i * 2 + 1;
    }
    return i == k;
}


template <class T>
void RefMinHeap<T>::push(size_t ref, T t) {
    if (ref_.find(ref) == ref_.end()) {
        ref_[ref] = heap_.size();
        heap_.push_back(t);
        swim_(ref_[ref]);
    } else {
        modify(ref, t);
    }

}


template <class T>
T RefMinHeap<T>::top() const {
    return heap_.front();
}

template <class T>
void RefMinHeap<T>::pop() {
    delete_(0);
}


template <class T>
void RefMinHeap<T>::delete_(size_t index) {
    ref_.erase(heap_[index].get_ref());
    size_t end = heap_.size() - 1;
    if (index != end) {
        swap_(end, index);
        if (!sink_(index, end)) swim_(index);
    }
    heap_.pop_back();
}

template <class T>
void RefMinHeap<T>::modify(size_t ref, T t) {
    delete_(ref_[ref]);
    push(ref, t);
}



#endif //MYSERVER_REFMINHEAP_H
