// src/math/math_impl.cpp
module math;  // 注意：这里不加 export！

// 实现主模块中声明的函数
namespace math {
    int add(int a, int b) {
        return a + b;
    }

    int multiply(int a, int b) {
        return a * b;
    }
}