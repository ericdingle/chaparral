#include "bonavista/testing/inst_count.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(InstCountTest) {
};

TEST(InstCountTest, ConstructorAndDestructor) {
  EXPECT_EQ(InstCount::inst_count(), 0u);
  {
    InstCount count;
    EXPECT_EQ(InstCount::inst_count(), 1u);
  }
  EXPECT_EQ(InstCount::inst_count(), 0u);
}

class Dummy1 : public InstCountT<Dummy1> {};
class Dummy2 : public InstCountT<Dummy2> {};

TEST(InstCountTest, TemplateConstructorAndDestructor) {
  EXPECT_EQ(Dummy1::inst_count(), 0u);
  EXPECT_EQ(Dummy2::inst_count(), 0u);
  {
    Dummy1 dummy1;
    EXPECT_EQ(Dummy1::inst_count(), 1u);
    EXPECT_EQ(Dummy2::inst_count(), 0u);
  }
  EXPECT_EQ(Dummy1::inst_count(), 0u);
  EXPECT_EQ(Dummy2::inst_count(), 0u);
}
