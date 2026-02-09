#include <iostream>
#include <type_traits>

constexpr int compute(int x) {
  if (std::is_constant_evaluated()) {
    // 编译时求值的路径
    return x * x;
  } else {
    // 运行时求值的路径
    return x + x;
  }
}

int main() {
  constexpr int compile_time_result = compute(10); // 编译时求值，结果为100
  int runtime_result = compute(10); // 运行时求值，结果为20

  std::cout << "Compile time: " << compile_time_result << '\n';
  std::cout << "Run time: " << runtime_result << '\n';
}