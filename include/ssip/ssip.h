#pragma once

#include <vector>
#include <cstddef>
#include <stdexcept>
#include <atomic>

namespace ssip {
    template <class T>
    class RingBuffer {
        std::vector<T> vec_;
        std::size_t head_, tail_;
        
    public:
        
        RingBuffer(size_t capacity) {
            vec_ = std::vector<T>(capacity + 1);
            head_ = 0;
            tail_ = 0;
        }

        bool is_empty() {
            return head_ == tail_;
        }

        bool is_full() {
            return (head_ + 1) % vec_.size() == tail_;
        }

        T get() {
            if(is_empty()) throw std::out_of_range("buffer is empty");
            T ret = vec_[tail_];
            tail_ = (tail_ + 1) % vec_.size();
            return ret;
        }

        void put(T data) {
            if(is_full()) throw std::overflow_error("buffer is full");
            vec_[head_] = data;
            head_ = (head_ + 1) % vec_.size();
        }

        size_t size() {
            return (head_ + vec_.size() - tail_) % vec_.size();
        }

    };
}