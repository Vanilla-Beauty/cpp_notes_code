#include "asio/io_context.hpp"
#include "asio/ssl.hpp"
#include "asio/streambuf.hpp"
#include <asio.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>

using asio::ip::tcp;

class HttpServer {
public:
  HttpServer(asio::io_context &io_context, unsigned short port)
      : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    acceptor_.set_option(
        asio::ip::tcp::acceptor::reuse_address(true)); // 端口重用
    acceptor_.listen();                                // 监听端口
  }

  void Start() { Accept(); }

private:
  void Accept() {
    auto socket = std::make_shared<tcp::socket>(io_context_);
    acceptor_.async_accept(*socket, [this, socket](asio::error_code ec) {
      HandleAccept(ec, socket);
    });
  }
  void HandleAccept(asio::error_code &ec, std::shared_ptr<tcp::socket> socket) {
    if (!ec) {
      auto buffer = std::make_shared<asio::streambuf>();
      asio::async_read_until(
          *socket, *buffer, "\r\n\r\n", // 连续读到2个\r\n后认为请求头结束
          [this, socket, buffer](asio::error_code ec, std::size_t length) {
            HandleRequestHeaders(ec, socket, buffer);
          });
    }

    Accept(); // 设置读取回调之后, 需要再次设置Accept以处理其他请求
  }

  void HandleRequestHeaders(const asio::error_code &ec,
                            std::shared_ptr<tcp::socket> socket,
                            std::shared_ptr<asio::streambuf> buffer) {
    if (!ec) {
      std::istream request_stream(buffer.get());
      std::string request_line;
      std::getline(request_stream, request_line);

      std::string header;
      std::size_t content_length = 0;
      bool keep_alive = true;
      while (std::getline(request_stream, header) && header != "\r") {
        if (header.find("Content-Length:") == 0) {
          content_length = std::stoul(header.substr(15));
        }
        if (header.find("Connection:") == 0) {
          std::string connection = header.substr(11);
          if (connection.find("close") != std::string::npos) {
            keep_alive = false;
          }
        }
      }

      if (content_length > 0) {
        asio::async_read(
            *socket, *buffer, asio::transfer_exactly(content_length),
            // transfer_exactly 设置具体读取的字节数
            [this, socket, buffer, content_length,
             keep_alive](const asio::error_code &ec, std::size_t) {
              HandleRequestBody(ec, socket, buffer, content_length, keep_alive);
            });
      } else {
        HandleRequestBody(ec, socket, buffer, 0, keep_alive);
      }
    } else {
      std::cout << "Handle request headers error: " << ec.message() << "\n";
    }
  }

  void HandleRequestBody(const asio::error_code &ec,
                         std::shared_ptr<tcp::socket> socket,
                         std::shared_ptr<asio::streambuf> buffer,
                         std::size_t content_length, bool keep_alive) {
    if (!ec) {
      auto response = std::make_shared<asio::streambuf>();
      std::ostream response_stream(response.get());

      // 生成HTTP响应
      response_stream << "HTTP/1.1 200 OK\r\n"
                         "Content-Length: 13\r\n"
                         "Content-Type: text/plain\r\n"
                         "Connection: keep-alive\r\n"
                         "\r\n"
                         "Hello, world!";

      SendResponse(ec, socket, response, keep_alive);
    } else {
      std::cerr << "Request body read failed: " << ec.message() << "\n";
    }
  }

  void SendResponse(const asio::error_code &ec,
                    std::shared_ptr<tcp::socket> socket,
                    std::shared_ptr<asio::streambuf> response,
                    bool keep_alive) {
    if (!ec) {
      asio::async_write(*socket, *response,
                        [this, socket, response,
                         keep_alive](const asio::error_code &ec, std::size_t) {
                          if (!ec) {
                            if (keep_alive) {
                              // 继续读取新的请求
                              auto buffer = std::make_shared<asio::streambuf>();
                              asio::async_read_until(
                                  *socket, *buffer, "\r\n\r\n",
                                  [this, socket, buffer](
                                      const asio::error_code &ec, std::size_t) {
                                    HandleRequestHeaders(ec, socket, buffer);
                                  });
                            } else {
                              // 关闭连接
                              socket->shutdown(tcp::socket::shutdown_both);
                              socket->close();
                            }
                          } else {
                            std::cerr << "Write failed: " << ec.message()
                                      << "\n";
                          }
                        });
    } else {
      std::cerr << "Response send failed: " << ec.message() << "\n";
    }
  }

private:
  asio::io_context &io_context_;
  tcp::acceptor acceptor_;
};

int main() {
  try {
    asio::io_context io_context;

    // 创建HTTP服务器
    HttpServer server(io_context, 80); // HTTP标准端口为80
    server.Start();

    // 创建一个线程池
    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
      threads.emplace_back([&io_context]() { io_context.run(); });
    }

    // 主线程等待中断信号
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait(
        [&io_context](const asio::error_code &, int) { io_context.stop(); });

    // 等待所有线程完成
    for (auto &thread : threads) {
      thread.join();
    }
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}