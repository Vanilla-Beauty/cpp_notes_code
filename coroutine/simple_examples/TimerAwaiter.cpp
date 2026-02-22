// timer_awaiter.cpp
#include <chrono>
#include <coroutine>
#include <iostream>
#include <thread>

// ==================== TimerAwaiter 定义 ====================
struct TimerAwaiter {
  std::chrono::milliseconds duration;

  bool await_ready() const noexcept { return duration.count() <= 0; }

  void await_suspend(std::coroutine_handle<> h) {
    std::thread([h, duration = duration]() {
      std::this_thread::sleep_for(duration);
      h.resume();
    }).detach();
  }

  void await_resume() noexcept {
    std::cout << "  [Timer] 定时器完成!" << std::endl;
  }
};

// ==================== 协程返回类型定义 ====================
struct TimerTask {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  handle_type handle;

  struct promise_type {
    TimerTask get_return_object() {
      return TimerTask(handle_type::from_promise(*this));
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {
      std::cout << "  [Exception] 协程异常!" << std::endl;
    }
  };

  TimerTask(handle_type h) : handle(h) {}

  ~TimerTask() {
    if (handle)
      handle.destroy();
  }

  TimerTask(TimerTask &&other) noexcept : handle(other.handle) {
    other.handle = nullptr;
  }

  TimerTask &operator=(TimerTask &&other) noexcept {
    if (this != &other) {
      if (handle)
        handle.destroy();
      handle = other.handle;
      other.handle = nullptr;
    }
    return *this;
  }

  void start() {
    if (handle && handle.done() == false) {
      handle.resume();
    }
  }
};

// ==================== 协程函数 ====================
TimerTask delay(std::chrono::milliseconds ms, const std::string &name) {
  std::cout << "  [" << name << "] 开始等待 " << ms.count() << "ms"
            << std::endl;
  co_await TimerAwaiter{ms};
  std::cout << "  [" << name << "] 等待结束!" << std::endl;
}

// ==================== 主函数 ====================
int main() {
  std::cout << "========== TimerAwaiter 示例 ==========" << std::endl;

  auto task1 = delay(std::chrono::milliseconds(500), "Task1");
  auto task2 = delay(std::chrono::milliseconds(300), "Task2");

  std::cout << "  [Main] 启动协程..." << std::endl;
  task1.start();
  task2.start();

  // 等待所有协程完成
  std::this_thread::sleep_for(std::chrono::milliseconds(600));

  std::cout << "  [Main] 程序结束" << std::endl;
  return 0;
}
