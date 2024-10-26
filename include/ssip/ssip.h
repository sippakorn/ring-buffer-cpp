#pragma once

#include <cstddef>
#include <mutex>
#include <stdexcept>
#include <vector>

namespace ssip {
  template <class T> class RingBuffer {
    std::vector<T> vec_;
    std::size_t head_, tail_;
    std::mutex lock_mtx_;

  public:
    RingBuffer(size_t capacity) {
      vec_ = std::vector<T>(capacity + 1);
      head_ = 0;
      tail_ = 0;
    }

  private:
    // is_empty() without mutex lock
    bool is_empty_() { return head_ == tail_; }

    bool is_full_() { return (head_ + 1) % vec_.size() == tail_; }

  public:
    bool is_empty() {
      std::lock_guard<std::mutex> lock(lock_mtx_);
      return head_ == tail_;
    }

    bool is_full() {
      std::lock_guard<std::mutex> lock(lock_mtx_);
      return (head_ + 1) % vec_.size() == tail_;
    }

    T get() {
      std::lock_guard<std::mutex> lock(lock_mtx_);
      if (is_empty_()) throw std::out_of_range("buffer is empty");
      T ret = vec_[tail_];
      tail_ = (tail_ + 1) % vec_.size();
      return ret;
    }

    void put(T data) {
      std::lock_guard<std::mutex> lock(lock_mtx_);
      if (is_full_()) throw std::overflow_error("buffer is full");
      vec_[head_] = data;
      head_ = (head_ + 1) % vec_.size();
    }

    size_t size() {
      std::lock_guard<std::mutex> lock(lock_mtx_);
      return (head_ + vec_.size() - tail_) % vec_.size();
    }
  };
}  // namespace ssip