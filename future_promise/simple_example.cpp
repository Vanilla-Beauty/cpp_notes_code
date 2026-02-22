#include <chrono>
#include <future>
#include <iostream>
#include <thread>

void task(std::promise<int> prom) {
  // 模拟耗时操作
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // 设置结果
  prom.set_value(42);
}

int main() {
  std::promise<int> prom;
  std::future<int> fut = prom.get_future();

  // 启动线程
  std::thread t(task, std::move(prom));

  // wait 和 wait_for 可以多次调用，不消耗结果
  fut.wait_for(std::chrono::milliseconds(200));
  std::cout << "Waiting 200ms" << std::endl;

  // wait会阻塞直到结果就绪
  fut.wait();
  std::cout << "Ready!" << std::endl;

  // 等待并获取结果（会阻塞直到结果就绪）, get只能调用一次，并且会消耗结果
  int result = fut.get();

  std::cout << "Result: " << result << std::endl;

  t.join();
  return 0;
}