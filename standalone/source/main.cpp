#include <ssip/ssip.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

ssip::RingBuffer<int> rb(1024 * 2);
std::atomic<int> sum(0);

void producer() {
  std::cout << "Thread: " << std::this_thread::get_id() << "\n";
  std::cout << "Thread: " << std::this_thread::get_id() << " - size() before put => " << rb.size()
            << "\n";
  int i = 0;
  while (i < 10000) {
    try {
      rb.put(i);
      i++;
    } catch (std::overflow_error& e) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
  }
  std::cout << "Thread: " << std::this_thread::get_id() << " - size() after put => " << rb.size()
            << "\n";
}

void consumer() {
  std::cout << "Thread: " << std::this_thread::get_id() << "\n";
  std::cout << "Thread: " << std::this_thread::get_id() << " - size() before get => " << rb.size()
            << "\n";
  int i = 0;
  while (i < 5000) {
    try {
      auto elem = rb.get();
      sum.fetch_add(elem);
      i++;
    } catch (std::out_of_range& e) {
      std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
  }
  std::cout << "Thread: " << std::this_thread::get_id() << " - size() after get => " << rb.size()
            << "\n";
}

long sum_range(int begin, int end) { return end * (end + 1) / 2 - begin * (begin + 1) / 2; }

auto main() -> int {
  std::cout << "RingBuffer Multi Thread" << "\n";

  std::cout << "Sum of [0 - 10000) => " << sum_range(0, 9999) << "\n";

  // producer writes 0-10000 to RB (10000 entries)
  std::thread t1(producer);
  // first consumer reads 5000 entry (5000 entries)
  std::thread t2(consumer);
  // first consumer reads 5000 entry (5000 entries)
  std::thread t3(consumer);

  t1.join();
  t2.join();
  t3.join();

  std::cout << "total consumer sum => " << sum.load() << "\n";
  return 0;
}
