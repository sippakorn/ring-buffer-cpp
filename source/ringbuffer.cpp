#include <ssip/ssip.h>
#include <vector>
#include <stdexcept>

using namespace ssip;

RingBuffer::RingBuffer(size_t capacity) {
    vec_ = std::vector<int>(capacity + 1);
    head_ = 0;
    tail_ = 0;
}

bool RingBuffer::is_empty() {
    return head_ == tail_;
}

bool RingBuffer::is_full() {
    return (head_ + 1) % vec_.size() == tail_;
}

int RingBuffer::get() {
    if(is_empty()) throw std::out_of_range("buffer is empty");
    int ret = vec_[tail_];
    tail_ = (tail_ + 1) % vec_.size();
    return ret;
}

void RingBuffer::put(int data) {
    if(is_full()) throw std::overflow_error("buffer is full");
    vec_[head_] = data;
    head_ = (head_ + 1) % vec_.size();
}

size_t RingBuffer::size() {
    return (head_ + vec_.size() - tail_) % vec_.size();
}
