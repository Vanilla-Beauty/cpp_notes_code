#include <iostream>
int main() {
  // C++14
  auto f = [](auto x, auto y) { return x + y; };

  auto res = f(1, 2);

  std::cout << res << std::endl;
}