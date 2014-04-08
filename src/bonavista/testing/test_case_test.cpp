#include "bonavista/testing/test_case.h"

TEST_CASE(TestCaseTest) {
 protected:
  void SetUp() {
    a_ = 0;
  }

  int a_;
};

TEST(TestCaseTest, ExpectEq) {
  EXPECT_EQ(a_, 0);
  EXPECT_EQ(a_ == 0, true);
}

TEST(TestCaseTest, ExpectNotEq) {
  EXPECT_NOT_EQ(a_, 1);
  EXPECT_NOT_EQ(a_ == 1, true);
}

TEST(TestCaseTest, ExpectTrue) {
  bool b = true;
  EXPECT_TRUE(b);
  EXPECT_TRUE(b == true);
  EXPECT_TRUE(b != false);
}

TEST(TestCaseTest, ExpectFalse) {
  bool b = false;
  EXPECT_FALSE(b);
  EXPECT_FALSE(b == true);
  EXPECT_FALSE(b != false);
}

TEST(TestCaseTest, ExpectNull) {
  void* p = NULL;
  EXPECT_NULL(p);
}

TEST(TestCaseTest, ExpectNotNull) {
  void* p = reinterpret_cast<void*>(1);
  EXPECT_NOT_NULL(p);
}

TEST(TestCaseTest, ExpectBlock) {
  if (true)
    EXPECT_TRUE(true);
  else
    EXPECT_TRUE(false);

  if (false)
    EXPECT_TRUE(false);
  else
    EXPECT_TRUE(true);
}
