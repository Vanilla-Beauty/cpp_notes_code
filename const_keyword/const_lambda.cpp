#include <iostream>

using namespace std;

int main() {
  // constexpr lambda
  auto func1 = [](int value) constexpr {
    int ret = 1;
    for (int i = 2; i <= value; i++)
      ret *= i;
    return ret;
  };
  auto func2 = [](int v) consteval { return v * 2; };

  constexpr int v1 = func1(4) + func2(5);

  cout << v1 << endl;
}