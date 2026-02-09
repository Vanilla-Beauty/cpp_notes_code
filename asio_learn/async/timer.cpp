#include "asio/io_context.hpp"
#include <asio.hpp>
#include <chrono>
#include <iostream>

void TimerHandler(asio::error_code const &ec, asio::steady_timer &timer) {
  if (!ec) {
    std::cout << "Timer expired" << std::endl;

    // 重新启动定时器, 形成循环
    timer.expires_after(std::chrono::seconds(2));
    timer.async_wait(
        [&timer](asio::error_code const &ec) { TimerHandler(ec, timer); });

  } else {
    std::cout << "Timer error: " << ec.message() << std::endl;
  }
}

int main() {
  try {
    asio::io_context io_ctx;

    // 创建一个定时器, 设置2s超时
    asio::steady_timer timer(io_ctx, std::chrono::seconds(2));

    // 设定定时器回调函数
    timer.async_wait(
        [&timer](asio::error_code const &ec) { TimerHandler(ec, timer); });

    // 进入时间循环
    io_ctx.run();
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}