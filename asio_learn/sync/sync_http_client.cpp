#include "asio/error_code.hpp"
#include <asio.hpp>
#include <cstddef>
#include <iostream>

using asio::ip::tcp;

template <class T>
void HandleRequest(T &socket, const std::string &server,
                   const std::string &path) {
  std::string request = "GET " + path + " HTTP/1.0\r\n";
  request += "Host: " + server + "\r\n";
  request += "Accept: */*\r\n"
             "Connection: close\r\n\r\n";

  asio::write(socket, asio::buffer(request));

  char buffer[1024];
  asio::error_code error;

  while (size_t len = socket.read_some(asio::buffer(buffer), error)) {
    // 读取数据并输出到标准输出
    std::cout.write(buffer, len);
  }
}

void HttpRequests(const std::string &server, const std::string &path) {
  asio::io_context io_context; // 负责调度管理所有的异步操作

  tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(server, "http");

  tcp::socket socket(io_context);

  asio::connect(socket, endpoints);

  HandleRequest(socket, server, path);
}

int main() {
  const std::string server = "bilibili.com";
  const std::string path = "/";

  try {
    HttpRequests(server, path);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}