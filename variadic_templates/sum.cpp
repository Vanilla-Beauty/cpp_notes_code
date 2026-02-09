#include <iostream>
template <typename T> T sum(T arg) { return arg; }
template <typename T, typename... Ts> T sum(T arg, Ts... args) {
  return arg + sum(args...); // recursive call
}

// 折叠表达式版本
template <typename... Args> auto sum2(Args... args) -> decltype((args + ...)) {
  return (args + ...); // 左折叠
}

int main() {
  auto res = sum(1, 2, 3, 4, 5);
  std::cout << res << std::endl;

  auto res2 = sum2(1, 2, 3, 4, 5);
  std::cout << res2 << std::endl;
}