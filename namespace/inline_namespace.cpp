#include "iostream"

namespace Parent {
namespace Child1 {
void print_v1() { std::cout << "Parent::Child1::print_v1()" << std::endl; }
} // namespace Child1
inline namespace Child2 {
void print_v2() { std::cout << "Parent::Child2::print_v2()" << std::endl; }
} // namespace Child2
} // namespace Parent

int main() {
  Parent::print_v2(); // prints Parent::Child2::print()
  Parent::Child1::print_v1();
}