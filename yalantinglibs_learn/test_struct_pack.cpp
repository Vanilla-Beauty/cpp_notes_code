#include "ylt/struct_pack.hpp"
#include <fstream>
#include <gtest/gtest.h>

// 定义测试用的结构体
struct Person {
  std::string name;
  int age;
  double height;
};

// 用于测试嵌套结构体
struct Address {
  std::string street;
  std::string city;
};

struct Employee {
  Person person;
  Address address;
  int employee_id;
};

// 用于测试包含容器的结构体
struct Team {
  std::string team_name;
  std::vector<Person> members;
  std::optional<std::string> department;
};

TEST(StructPackTest, BasicSerializationDeserialization) {
  // 创建一个 Person 对象
  Person p1{.name = "Alice", .age = 30, .height = 1.75};

  // 序列化对象
  std::vector<char> serialized_data = struct_pack::serialize(p1);

  // 反序列化数据
  auto result = struct_pack::deserialize<Person>(serialized_data);
  ASSERT_TRUE(result.has_value());

  Person p2 = std::move(result.value());

  // 验证反序列化结果
  EXPECT_EQ(p2.name, "Alice");
  EXPECT_EQ(p2.age, 30);
  EXPECT_DOUBLE_EQ(p2.height, 1.75);
}

TEST(StructPackTest, StringSerialization) {
  Person p1{.name = "Bob", .age = 25, .height = 1.80};

  // 序列化到 std::string
  std::string serialized_string = struct_pack::serialize<std::string>(p1);

  // 反序列化
  auto result = struct_pack::deserialize<Person>(serialized_string);
  ASSERT_TRUE(result.has_value());

  Person p2 = std::move(result.value());

  EXPECT_EQ(p2.name, "Bob");
  EXPECT_EQ(p2.age, 25);
  EXPECT_DOUBLE_EQ(p2.height, 1.80);
}

TEST(StructPackTest, NestedStructures) {
  Employee emp{.person = Person{.name = "Charlie", .age = 35, .height = 1.78},
               .address = Address{.street = "123 Main St", .city = "New York"},
               .employee_id = 1001};

  // 序列化嵌套结构体
  std::vector<char> serialized_data = struct_pack::serialize(emp);

  // 反序列化
  auto result = struct_pack::deserialize<Employee>(serialized_data);
  ASSERT_TRUE(result.has_value());

  Employee emp2 = std::move(result.value());

  EXPECT_EQ(emp2.person.name, "Charlie");
  EXPECT_EQ(emp2.person.age, 35);
  EXPECT_DOUBLE_EQ(emp2.person.height, 1.78);
  EXPECT_EQ(emp2.address.street, "123 Main St");
  EXPECT_EQ(emp2.address.city, "New York");
  EXPECT_EQ(emp2.employee_id, 1001);
}

TEST(StructPackTest, ContainerTypes) {
  Team team{.team_name = "Engineering",
            .members = {Person{.name = "David", .age = 28, .height = 1.82},
                        Person{.name = "Eve", .age = 32, .height = 1.68}},
            .department = "R&D"};

  // 序列化包含容器的结构体
  std::vector<char> serialized_data = struct_pack::serialize(team);

  // 反序列化
  auto result = struct_pack::deserialize<Team>(serialized_data);
  ASSERT_TRUE(result.has_value());

  Team team2 = std::move(result.value());

  EXPECT_EQ(team2.team_name, "Engineering");
  ASSERT_EQ(team2.members.size(), 2);

  EXPECT_EQ(team2.members[0].name, "David");
  EXPECT_EQ(team2.members[0].age, 28);
  EXPECT_DOUBLE_EQ(team2.members[0].height, 1.82);

  EXPECT_EQ(team2.members[1].name, "Eve");
  EXPECT_EQ(team2.members[1].age, 32);
  EXPECT_DOUBLE_EQ(team2.members[1].height, 1.68);

  ASSERT_TRUE(team2.department.has_value());
  EXPECT_EQ(team2.department.value(), "R&D");
}

TEST(StructPackTest, DeserializeTo) {
  Person p1{.name = "Frank", .age = 40, .height = 1.75};

  std::vector<char> serialized_data = struct_pack::serialize(p1);

  Person p2;
  auto ec = struct_pack::deserialize_to(p2, serialized_data);

  EXPECT_EQ(ec, struct_pack::errc{});
  EXPECT_EQ(p2.name, "Frank");
  EXPECT_EQ(p2.age, 40);
  EXPECT_DOUBLE_EQ(p2.height, 1.75);
}

TEST(StructPackTest, SizeAndAlignment) {
  // 序列化到指定地址
  Person p{.name = "Grace", .age = 35, .height = 1.70};

  // 测试获取序列化大小
  auto size = struct_pack::get_needed_size(p);
  std::vector<char> buffer(size);

  // 测试序列化到预分配的缓冲区
  struct_pack::serialize_to(buffer.data(), size, p);

  // 验证反序列化
  auto deserialized =
      struct_pack::deserialize<Person>(buffer.data(), buffer.size());
  ASSERT_TRUE(deserialized.has_value());

  Person p2 = std::move(deserialized.value());
  EXPECT_EQ(p2.name, "Grace");
  EXPECT_EQ(p2.age, 35);
  EXPECT_DOUBLE_EQ(p2.height, 1.70);
}

TEST(StructPackTest, SerializeToStd) {
  Person p{.name = "Grace", .age = 35, .height = 1.70};
  std::ofstream writer("struct_pack_demo.data",
                       std::ofstream::out | std::ofstream::binary);
  struct_pack::serialize_to(writer, p);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}