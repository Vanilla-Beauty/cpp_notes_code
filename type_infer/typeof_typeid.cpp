#include <iostream>

const int &&foo() { return 42; }
int main() {
  int a = 42;

  //   // 1. typeof: gcc 扩展
  //   typeof(a) b = 1; // clang 报错, gcc正常编译
  //   std::cout << "b = " << b << std::endl;

  // 2. typeid: c++11
  int x1 = 0;
  double x2 = 5.5;
  long long x3 = 50;
  std::cout << "typeid(x1) = " << typeid(x1).name() << std::endl;
  std::cout << "typeid(x1+x2) = " << typeid(x1 + x2).name() << std::endl;
  std::cout << "typeid(int) = " << typeid(int).name() << std::endl;
  std::cout << "typeid(x3) = " << typeid(x3).name() << std::endl;

  //   typeid(x1) x11 = 10; // ! 不能用于声明变量, 因为其只在运行时生效,
  //   无法在编译器推导类型
}