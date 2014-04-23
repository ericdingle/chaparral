#include "chaparral/executer/variant.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(VariantTest) {
};

TEST(VariantTest, GetPrimitive) {
  Variant var(5);

  int i = 0;
  EXPECT_TRUE(var.Get(&i));
  EXPECT_EQ(5, i);

  double d;
  EXPECT_FALSE(var.Get(&d));
}

class Dummy {
};

class Dummy2 {
};

TEST(VariantTest, GetObject) {
  shared_ptr<Dummy> dummy(new Dummy());
  EXPECT_EQ(1, dummy.use_count());

  Variant var(dummy);
  EXPECT_EQ(2, dummy.use_count());

  shared_ptr<Dummy> dummy1;
  EXPECT_TRUE(var.Get(&dummy1));
  EXPECT_EQ(dummy.get(), dummy1.get());
  EXPECT_EQ(3, dummy.use_count());

  shared_ptr<Dummy2> dummy2;
  EXPECT_FALSE(var.Get(&dummy2));
}
