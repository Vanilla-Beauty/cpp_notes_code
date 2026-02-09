// src//math/math.ixx
export module math;

// 导入模块分区
export import :utils;  // 等价于 import math:utils;

export namespace math {
    // 导出函数声明（定义在实现单元）
    int add(int a, int b);
    int multiply(int a, int b);

    // 内联函数可直接定义并导出
    inline int power2(int x) {
        return utils::square(x);
    }
}