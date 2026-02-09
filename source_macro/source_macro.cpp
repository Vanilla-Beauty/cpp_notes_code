#include <iostream>

void log(const std::string &message) {
  std::cout << "File:" << __FILE__ << ", Line:" << __LINE__
            << ", func:" << __func__
            << ", PRETTY_FUNCTION:" << __PRETTY_FUNCTION__ << '\n';
}

int main() { log("Hello, World!"); }
