#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

// 任务函数
int compute(int id) {
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  return id * 10;
}

int main() {
  // 1. 使用 async 并行执行多个任务
  std::vector<std::future<int>> futures;
  for (int i = 0; i < 5; ++i) {
    futures.push_back(std::async(std::launch::async, compute, i));
  }

  // 2. 收集结果
  std::cout << "Results: ";
  for (auto &fut : futures) {
    std::cout << fut.get() << " ";
  }
  std::cout << std::endl;

  // 3. 使用 promise/future 自定义通信
  std::promise<std::string> prom;
  std::future<std::string> fut = prom.get_future();

  std::thread t([&prom]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    prom.set_value("Hello from thread!");
  });

  std::cout << fut.get() << std::endl;
  t.join();

  // 4. 超时处理示例
  auto slow_fut = std::async([]() {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 42;
  });

  if (slow_fut.wait_for(std::chrono::seconds(1)) == std::future_status::ready) {
    std::cout << "Result: " << slow_fut.get() << std::endl;
  } else {
    std::cout << "Task still running..." << std::endl;
  }

  return 0;
}