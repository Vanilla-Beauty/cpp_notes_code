struct Empty {}; // 空类型

struct WithoutNoUniqueAddress {
  int data;
  Empty e; // 即使是空类型，也会占用空间
};

struct WithNoUniqueAddress {
  int data;
  [[no_unique_address]] Empty e; // 不会占用额外空间
};

int main() {
  // 通常情况下，空类型成员会占用至少1字节的空间
  static_assert(sizeof(WithoutNoUniqueAddress) > sizeof(int));
  // 使用了[[no_unique_address]]，空类型成员可以不占用空间
  static_assert(sizeof(WithNoUniqueAddress) == sizeof(int));
}