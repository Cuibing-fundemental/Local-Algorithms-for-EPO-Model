#pragma once
#include <cstddef>

struct MyQueue{
    size_t *data_;
    int front_ = 0;
    int back_ = 0;
    int capacity_ = 0;

    MyQueue(size_t n);
    void push(size_t v);
    size_t pop();
    bool empty() const;
    size_t size() const;
};