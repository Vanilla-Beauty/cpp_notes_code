#include <iostream>
#include <source_location>

void log(const std::string &message, const std::source_location &location =
                                         std::source_location::current()) {
  std::cout << "File:" << location.file_name() << ", Line:" << location.line()
            << ", Message:" << message << '\n';
}

int main() { log("Hello, World!"); }
