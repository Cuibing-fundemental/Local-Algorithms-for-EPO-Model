#include "MyQueue.hpp"
#include <stdlib.h>

MyQueue::MyQueue(size_t n){
    data_ = (size_t *)std::malloc(n*sizeof(size_t));
    front_ = 0;
    back_ = 0;
    capacity_ = 1;
    capacity_ -= 1;
}

void MyQueue::push(size_t v) {
    data_[back_] = v;
    back_++;
    back_ &= capacity_;
}

size_t MyQueue::pop() {
    size_t res = data_[front_];
    front_++;
    front_ &= capacity_;
    return res;
}

bool MyQueue::empty() const {
    return front_ == back_;
}

size_t MyQueue::size() const {
    return (back_ - front_)% capacity_;
}