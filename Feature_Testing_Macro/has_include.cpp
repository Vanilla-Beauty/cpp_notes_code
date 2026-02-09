#if __has_include(<iostream>)
#include <iostream>
#endif

#if __cpp_constexpr
constexpr int square(int x) { return x * x; }
#endif