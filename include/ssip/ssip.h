#pragma once

#include <vector>
#include <cstddef>

namespace ssip {
    
    class RingBuffer {
        std::vector<int> vec_;
        std::size_t head_;
        size_t tail_;
        
        public:
            RingBuffer(size_t);
            int get();
            void put(int);
            size_t size();
            bool is_empty();
            bool is_full();

    };
}