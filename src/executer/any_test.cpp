#include "executer/any.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(AnyTest, Empty) {
  Any any;
  int i;
  EXPECT_FALSE(any.Get(&i));
}

TEST(AnyTest, CopyConstructor) {
  Any any(5);
  Any any2(any);
  int i;
  EXPECT_TRUE(any.Get(&i));
  EXPECT_EQ(5, i);
  EXPECT_TRUE(any2.Get(&i));
  EXPECT_EQ(5, i);
}

TEST(AnyTest, MoveConstructor) {
  Any any(5);
  Any any2 = std::move(any);
  int i;
  EXPECT_FALSE(any.Get(&i));
  EXPECT_TRUE(any2.Get(&i));
  EXPECT_EQ(5, i);
}

TEST(AnyTest, AssignmentOperator) {
  Any any(5);
  Any any2;
  int i;
  EXPECT_FALSE(any2.Get(&i));

  any2 = any;
  EXPECT_TRUE(any2.Get(&i));
  EXPECT_EQ(5, i);
}

TEST(AnyTest, Get) {
  Any any(5);

  int i = 0;
  EXPECT_TRUE(any.Get(&i));
  EXPECT_EQ(5, i);

  double d;
  EXPECT_FALSE(any.Get(&d));
}

TEST(AnyTest, GetSharedPtr) {
  auto i = std::make_shared<int>(5);
  EXPECT_EQ(1, i.use_count());

  Any any(i);
  EXPECT_EQ(2, i.use_count());

  std::shared_ptr<int> j;
  EXPECT_TRUE(any.Get(&j));
  EXPECT_EQ(i.get(), j.get());
  EXPECT_EQ(3, i.use_count());

  std::shared_ptr<double> d;
  EXPECT_FALSE(any.Get(&d));
}
