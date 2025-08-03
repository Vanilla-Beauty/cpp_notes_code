#include <iostream>
#include <type_traits>

// 本代码的模板: 自定义一个打印函数, 且需要展开浮点数类型的指针

template <typename T> struct strip_pointer {
  using type = T;
};

template <typename T> struct strip_pointer<T *> {
  using type = T;
};

template <typename T> void _my_print(T t) {
  using T_without_pointer = typename strip_pointer<T>::type;
  //   !typename关键字的作用是告诉编译器strip_pointer<T>::type是一个类型名，而不是一个变量名或函数名。
  if constexpr (std::is_pointer<T>::value &&
                std::is_floating_point<T_without_pointer>::value) {
    // ! if constexpr在编译时判断，不满足条件的分支会被忽略，不参与模板实例化
    // ! 普通if在编译时两个分支都会被编译，只是在运行时选择执行哪一条路径
    std::cout << *t;
  } else {
    std::cout << t;
  }
}

template <typename A, typename B, typename C> void my_print(A a, B b, C c) {
  _my_print(a);
  std::cout << ", ";
  _my_print(b);
  std::cout << ", ";
  _my_print(c);
  std::cout << std::endl;
}

int main() {
  double f1 = 3.14;
  std::string s1 = "Hello";
  my_print(1, &s1, &f1);
}