#include <iostream>

// 示例 1：求和（一元右折叠）
template <typename... Args> auto sum(Args... args) {
  return (args + ...); // 一元右折叠
}

// 示例 2：求积（一元左折叠）
template <typename... Args> auto product(Args... args) {
  return (... * args); // 一元左折叠
}

// 示例 3：逻辑与（二元右折叠）
template <typename... Args> bool all_true(Args... args) {
  return (args && ... && true); // 二元右折叠
}

// 示例 4：字符串拼接（二元左折叠）
template <typename... Args> std::string concatenate(Args... args) {
  return (std::string() + ... + args); // 二元左折叠
}

int main() {
  std::cout << sum(1, 2, 3, 4, 5) << std::endl;         // 输出：15
  std::cout << product(1, 2, 3, 4, 5) << std::endl;     // 输出：120
  std::cout << all_true(true, true, true) << std::endl; // 输出：true
  std::cout << concatenate("Hello, ", "world!", " ", "C++17") << std::endl;
  // 输出：Hello, world! C++17

  return 0;
}