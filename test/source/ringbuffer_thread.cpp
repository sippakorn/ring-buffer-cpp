#include <doctest/doctest.h>
#include <ssip/ssip.h>

#include <atomic>
#include <thread>

ssip::RingBuffer<int> rb(2048);
std::atomic<int> actual_sum(0);

void producer() {
  int i = 0;
  while (i < 10000) {
    try {
      rb.put(i);
      i++;
    } catch (std::overflow_error& e) {
      std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
  }
}

void consumer() {
  int i = 0;
  while (i < 5000) {
    try {
      auto elem = rb.get();
      actual_sum.fetch_add(elem);
      i++;
    } catch (std::out_of_range& e) {
      std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
  }
}

long sum_range(int begin, int end) { return end * (end + 1) / 2 - begin * (begin + 1) / 2; }

void test_single_thread() {
  CHECK(rb.size() == 0);
  CHECK(rb.is_empty());
  CHECK(!rb.is_full());
  for (int i = 0; i < 2046; i++) {
    rb.put(i);
  }
  CHECK(rb.size() == 2046);
  CHECK(!rb.is_empty());
  rb.put(2046);
  rb.put(2047);
  CHECK(rb.size() == 2048);
  CHECK(rb.is_full());
  rb.get();  // a[0] => 0
  rb.get();  // a[1] => 1
  rb.get();  // a[2] => 2
  CHECK(rb.size() == 2045);
  rb.get();              // a[3] => 3
  CHECK(rb.get() == 4);  // a[4] => 4
  CHECK(!rb.is_empty());
}

TEST_CASE("given new int RB with single thread") {
  std::thread t1(test_single_thread);
  t1.join();
}

TEST_CASE("given new int RB with one producer thread and two consumer threads") {
  // Given
  long expected_sum = sum_range(0, 9999);

  // When
  // producer writes 0-10000 to RB (10000 entries)
  std::thread t1(producer);
  // first consumer reads 5000 entry (5000 entries)
  std::thread t2(consumer);
  // first consumer reads 5000 entry (5000 entries)
  std::thread t3(consumer);

  t1.join();
  t2.join();
  t3.join();
  // Then
  CHECK(expected_sum == actual_sum);
}