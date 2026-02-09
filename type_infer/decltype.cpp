#include <iostream>

template <typename T> T max(T a, T b) { return a > b ? a : b; }

int main() {
  // 1. 推导一个已经存在的变量
  int x1 = 1;
  decltype(x1) x2 = x1;
  std::cout << "x2 = " << x2 << std::endl;

  // 2. 推导函数返回值
  decltype(max<int>(1, 2)) x3 = 3;
  std::cout << "x3 = " << x3 << std::endl;
}