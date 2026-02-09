#include <iostream>
#include <type_traits>

using namespace std;

template <typename T> auto process(const T &value) {
  if constexpr (std::is_integral<T>()) {
    cout << "传入的是整数" << endl;
    return value + 1;
  } else {
    value.coredump(); // 这个函数不存在，如果这一行回编译，肯定炸了
  }
}

int main() {
  cout << process(5) << endl;
  // cout << process(5.0) << endl; // 这一行会编译失败，因为 double 不是整数类型， coredump 函数不存在
}