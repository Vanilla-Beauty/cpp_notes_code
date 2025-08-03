// nested_namespace
#include <iostream>

// 原来的嵌套方式
namespace outer1 {
namespace inner1 {
void print() { std::cout << "outer2::inner2" << std::endl; }
} // namespace inner1
} // namespace outer1

// C++11 的嵌套方式
namespace outer2::inner2 {
void print() { std::cout << "outer2::inner2" << std::endl; }
} // namespace outer2::inner2

// 内联嵌套方式
namespace outer3::middle3::inline inner3 {
void print() { std::cout << "outer3::middle3::inline inner3" << std::endl; }
} // namespace outer3::middle3::inline inner3

int main() {
  outer1::inner1::print();
  outer2::inner2::print();
  outer3::middle3::print(); // 内联只有一层
                            // outer3::print(); // 报错
  return 0;
}