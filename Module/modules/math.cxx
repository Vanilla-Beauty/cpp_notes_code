// Global module fragment
module;

// Include standard library headers here if needed for the module implementation
#define PI 3.14159265358979323846
#include <cmath> // For std::pow

// Define and export the module
export module math;

export template <typename T> T add(T a, T b) { return a + b; }

export template <typename T> T multiply(T a, T b) { return a * b; }

export template <typename T> T divide(T a, T b) { return a / b; }

export template <typename T> T square(T a) { return a * a; }

export double power(double a, double b) {
  return std::pow(a, b);
} // Exported function (public API)

// Non-exported function (private to the module)
template <typename T> T sub(T a, T b) { return a - b; }
