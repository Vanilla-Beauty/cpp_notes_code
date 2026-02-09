#include <bit>
#include <iostream>

int main() {
  float f = 3.14159f;
  // 将float类型的变量f的位表示转换为无符号整数
  auto bits = std::bit_cast<unsigned int>(f);

  std::cout << "Float value: " << f << '\n';
  std::cout << "Bitwise representation: " << std::hex << bits << '\n';
}