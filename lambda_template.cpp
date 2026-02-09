#include <iostream>
#include <type_traits>

using namespace std;

int main() {
  auto func1 = []<typename T>(T val) { cout << val << endl; };

  func1(5);

  auto func2 = []<typename T>(T val)
    requires std::is_arithmetic_v<T>
  {
    cout << val << endl;
  };

  func2(5.4);
}