#include <future>
#include <iostream>

int compute() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 42;
}

int main() {
  // async 自动创建 promise/future
  std::future<int> fut = std::async(std::launch::async, compute);
  // std::launch::async 强制在新线程中执行，std::launch::deferred 则在调用
  // get/wait 时执行

  // 获取结果（阻塞等待）
  while (true) {
    // wait_for 返回值可以用来检查状态
    if (fut.wait_for(std::chrono::milliseconds(200)) ==
        std::future_status::ready) {
      break;
    }
    std::cout << "Waiting for result..." << std::endl;
  }
  int result = fut.get();

  std::cout << "Result: " << result << std::endl;
  return 0;
}