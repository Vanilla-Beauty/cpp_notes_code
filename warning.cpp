#include <iostream>
using namespace std;

#define SELFWARN

#if defined(SELFWARN)
#warning "自定义警告"
#endif

int main() { cout << "hello world" << endl; }