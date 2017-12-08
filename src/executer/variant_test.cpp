#include "executer/variant.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(VariantTest, Get) {
  Variant var(5);

  int i = 0;
  EXPECT_TRUE(var.Get(&i));
  EXPECT_EQ(5, i);

  double d;
  EXPECT_FALSE(var.Get(&d));
}

TEST(VariantTest, GetSharedPtr) {
  std::shared_ptr<int> i(new int(5));
  EXPECT_EQ(1, i.use_count());

  Variant var(i);
  EXPECT_EQ(2, i.use_count());

  std::shared_ptr<int> j;
  EXPECT_TRUE(var.Get(&j));
  EXPECT_EQ(i.get(), j.get());
  EXPECT_EQ(3, i.use_count());

  std::shared_ptr<double> d;
  EXPECT_FALSE(var.Get(&d));
}
