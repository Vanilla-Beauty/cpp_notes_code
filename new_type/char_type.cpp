#include <iostream>
#include <locale>

int main() {
  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale());

  char16_t u16_str[] = u"你好";
  char32_t u32_str[] = U"你好";

  // 输出 char16_t 字符串
  // ! 需要输出宽字符，可以将 char16_t 和 char32_t 转换为 wchar_t，然后使用
  // ! std::wcout 输出。

  for (char16_t c : u16_str) {
    std::wcout << static_cast<wchar_t>(c);
  }
  std::wcout << std::endl;

  // 输出 char32_t 字符串
  for (char32_t c : u32_str) {
    std::wcout << static_cast<wchar_t>(c);
  }
  std::wcout << std::endl;

  return 0;
}