#include <iostream>
#include <type_traits>

// 用法1: 选择不同的模板函数
// 只有当 T 是整数类型时，该函数才启用
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void print_integer(T value) {
  std::cout << "Integer: " << value << std::endl;
}

// 只有当 T 是浮点数类型时，该函数才启用
template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              int>::type = 0>
void print_float(T value) {
  std::cout << "Float: " << value << std::endl;
}

// 用法2: 用于函数的不同返回值
// 如果 T 是整数类型，返回 T；否则返回 void
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type process(T value) {
  return value * 2;
}

// 用法3: 构造函数
class MyClass {
public:
  // 只有当 T 是整数类型时，该构造函数才启用
  template <typename T,
            typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
  MyClass(T value) {
    std::cout << "Constructed with integer: " << value << std::endl;
  }
};

// 用法4: 结合 is_same
// 只有当 T 是 int 类型时，该函数才启用
template <typename T,
          typename std::enable_if<std::is_same<T, int>::value, int>::type = 0>
void print_int(T value) {
  std::cout << "This is an int: " << value << std::endl;
}

int main() {
  print_integer(42); // 输出：Integer: 42
  print_float(3.14); // 输出：Float: 3.14
  // print_integer(3.14);  // 编译错误：没有匹配的函数

  std::cout << process(10) << std::endl; // 输出：20

  MyClass obj(11);

  print_int(111);

  return 0;
}