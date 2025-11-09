// src/main.cpp
import math;           // 导入我们自己的模块
import <iostream>;      // C++20 标准库模块（需编译器支持）

int main() {
    using namespace std;
    using namespace math;

    cout << "2 + 3 = " << add(2, 3) << endl;
    cout << "4 * 5 = " << multiply(4, 5) << endl;
    cout << "7^2 = " << power2(7) << endl;      // 来自内联定义
    cout << "9^2 (via utils) = " << utils::square(9) << endl;

    return 0;
}