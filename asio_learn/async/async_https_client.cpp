
#include "asio/ssl.hpp"
#include <asio.hpp>
#include <iostream>
#include <ostream>

using asio::ip::tcp;

class AsyncHttpsClient {
public:
  AsyncHttpsClient(asio::io_context &io_context,
                   asio::ssl::context &ssl_context, const std::string &server,
                   const std::string &path)
      : resolver_(io_context), socket_(io_context, ssl_context),
        server_(server), path_(path) {}
  void Start() { Resolve(); }

private:
  void Resolve() {
    resolver_.async_resolve(server_, "https",
                            // 解析成功后通过任务链模式调用connect函数
                            [this](const asio::error_code &ec,
                                   const tcp::resolver::results_type &results) {
                              if (!ec) {
                                Connect(results);
                              } else {
                                std::cerr << "Resolve failed: " << ec.message()
                                          << "\n";
                              }
                            });
  }
  void Connect(const tcp::resolver::results_type &endpoints) {
    asio::async_connect(
        socket_.lowest_layer(), endpoints,
        // 连接成功后通过任务链模式调用Handshake函数
        [this](const asio::error_code &ec, const tcp::endpoint &) {
          if (!ec) {
            Handshake();
          } else {
            std::cerr << "Connect failed: " << ec.message() << "\n";
          }
        });
  }
  void Handshake() {
    socket_.async_handshake(asio::ssl::stream_base::client,
                            // ssl握手成功后通过任务链模式调用SendRequest函数
                            [this](const asio::error_code &ec) {
                              if (!ec) {
                                SendRequest();
                              } else {
                                std::cerr
                                    << "Handshake failed: " << ec.message()
                                    << "\n";
                              }
                            });
  }
  void SendRequest() {
    std::ostream request_stream(&request_);
    request_stream << "GET " << path_ << " HTTP/1.1\r\n";
    request_stream << "Host: " << server_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    asio::async_write(socket_, request_,
                      // 请求发送成功后通过任务链模式调用ReceiveResponse函数
                      [this](const asio::error_code &ec, std::size_t) {
                        if (!ec) {
                          ReceiveResponse();
                        } else {
                          std::cerr << "Request failed: " << ec.message()
                                    << "\n";
                        }
                      });
  }
  void ReceiveResponse() {
    asio::async_read(
        socket_, response_, [this](const asio::error_code &ec, std::size_t) {
          if (!ec || ec == asio::error::eof) {
            std::cout << &response_;
          } else {
            std::cerr << "Response failed: " << ec.message() << "\n";
          }
        });
  }

private:
  tcp::resolver resolver_;
  asio::ssl::stream<tcp::socket> socket_;
  std::string server_;
  std::string path_;
  asio::streambuf response_;
  asio::streambuf request_;
};

int main() {
  try {
    asio::io_context io_context;
    asio::ssl::context ssl_context(asio::ssl::context::sslv23);

    ssl_context.set_default_verify_paths();
    ssl_context.set_verify_mode(asio::ssl::verify_peer);

    std::string server = "bilibili.com";
    std::string path = "/";

    AsyncHttpsClient client(io_context, ssl_context, server, path);
    client.Start();   // 开启异步调用的任务链
    io_context.run(); // 进入时间循环
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
