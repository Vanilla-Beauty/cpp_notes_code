#include <iostream>
#include <list>
#include <ranges>
#include <string>
#include <vector>

namespace views = std::views;

// pipe
void example1() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};

  // Create a view that filters even numbers
  auto even_numbers = numbers |
                      views::filter([](int n) { return n % 2 == 1; }) |
                      views::transform([](int n) { return n * n; });

  // Print the even numbers
  for (int n : even_numbers) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
}

// take
void example2() {
  std::vector<std::string> words = {"apple", "banana", "cherry", "date"};

  auto filtered_words = words | views::transform([](const std::string &word) {
                          std::string upper;
                          for (char c : word) {
                            upper += std::toupper(c);
                          }
                          return upper;
                        }) |
                        views::take(3);

  for (const std::string &word : filtered_words) {
    std::cout << word << " ";
  }
  std::cout << std::endl;
}

void example3() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};

  // Create a view that filters even numbers
  auto even_numbers = numbers |
                      views::filter([](int n) { return n % 2 == 1; }) |
                      std::ranges::to<std::list>(); // ! cpp 23

  // Print the even numbers
  for (int n : even_numbers) {
    std::cout << n << " ";
  }
  std::cout << std::endl;
}

int main() {
  example1();
  example2();
  example3();
  return 0;
}