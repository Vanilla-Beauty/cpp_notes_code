#include <iostream>
#include <string>

/**
C++17: auto 用于非类型模板形参占位符

在 C++17 中，auto 可以作为 ​​非类型模板形参（non-type
template
parameter）的占位符​​，这意味着编译器可以自动推导非类型模板参数的具体类型。这是
C++17 引入的一项重要特性（P0127R2）。

​​什么是非类型模板参数？​​
非类型模板参数是指模板参数不是类型（如 typename T），而是具体的值，例如：

整型（int, char, size_t 等）
指针、引用
枚举
C++20 后还支持浮点类型和某些字面量类类型（如 std::nullptr_t）
*/

template <auto N> void print() { std::cout << N << std::endl; }

int main() {
  print<42>();
  print<'a'>();
  print<3.14>();
  print<true>();
  print<nullptr>();
  return 0;
}