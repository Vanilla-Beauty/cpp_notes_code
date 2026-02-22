#include <future>
#include <iostream>
#include <thread>
#include <vector>

int main() {
  std::promise<int> prom;
  std::shared_future<int> shared_fut = prom.get_future().share();

  // 多个线程可以共享同一个 future
  std::vector<std::thread> threads;
  for (int i = 0; i < 5; ++i) {
    threads.emplace_back([shared_fut, i]() {
      int value = shared_fut.get(); // 可以多次调用
      std::cout << "Thread " << i << ": " << value << std::endl;
    });
  }

  prom.set_value(42);

  for (auto &t : threads) {
    t.join();
  }

  return 0;
}