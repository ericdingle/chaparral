#include "executer/any.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(AnyTest, Get) {
  Any any(5);

  int i = 0;
  EXPECT_TRUE(any.Get(&i));
  EXPECT_EQ(5, i);

  double d;
  EXPECT_FALSE(any.Get(&d));
}

TEST(AnyTest, GetSharedPtr) {
  std::shared_ptr<int> i(new int(5));
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
