// async_result_awaiter.cpp
#include <atomic>
#include <chrono>
#include <coroutine>
#include <iostream>
#include <thread>

// ==================== AsyncResultAwaiter 定义 ====================
struct AsyncResultAwaiter {
  std::atomic<int> *result_ptr;

  bool await_ready() const noexcept {
    return result_ptr && result_ptr->load() != -1;
  }

  void await_suspend(std::coroutine_handle<> h) {
    std::thread([h, this]() {
      // 模拟异步计算
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      result_ptr->store(42); // 设置结果
      h.resume();            // 恢复协程
    }).detach();
  }

  int await_resume() noexcept { return result_ptr->load(); }
};

// ==================== 协程返回类型定义 ====================
template <typename T> struct AsyncTask {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    T value;
    std::exception_ptr exception;

    AsyncTask get_return_object() {
      return AsyncTask(handle_type::from_promise(*this));
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }

    void return_value(T v) { value = v; }

    void unhandled_exception() { exception = std::current_exception(); }
  };

  handle_type handle;

  AsyncTask(handle_type h) : handle(h) {}

  ~AsyncTask() {
    if (handle)
      handle.destroy();
  }

  AsyncTask(AsyncTask &&other) noexcept : handle(other.handle) {
    other.handle = nullptr;
  }

  void start() {
    if (handle && handle.done() == false) {
      handle.resume();
    }
  }

  T get_result() {
    if (handle.done()) {
      return handle.promise().value;
    }
    return T{};
  }
};

template <> struct AsyncTask<void> {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    std::exception_ptr exception;

    AsyncTask get_return_object() {
      return AsyncTask(handle_type::from_promise(*this));
    }
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() { exception = std::current_exception(); }
  };

  handle_type handle;

  AsyncTask(handle_type h) : handle(h) {}

  ~AsyncTask() {
    if (handle)
      handle.destroy();
  }

  AsyncTask(AsyncTask &&other) noexcept : handle(other.handle) {
    other.handle = nullptr;
  }

  void start() {
    if (handle && handle.done() == false) {
      handle.resume();
    }
  }
};

// ==================== 协程函数 ====================
AsyncTask<int> fetch_data() {
  std::cout << "  [Coroutine] 开始获取数据..." << std::endl;
  std::atomic<int> result{-1};
  int value = co_await AsyncResultAwaiter{&result};
  std::cout << "  [Coroutine] 获取到数据: " << value << std::endl;
  co_return value;
}

// ==================== 主函数 ====================
int main() {
  std::cout << "========== AsyncResultAwaiter 示例 ==========" << std::endl;

  auto task = fetch_data();

  std::cout << "  [Main] 启动协程..." << std::endl;
  task.start();

  // 等待协程完成
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  std::cout << "  [Main] 最终结果: " << task.get_result() << std::endl;
  std::cout << "  [Main] 程序结束" << std::endl;

  return 0;
}
