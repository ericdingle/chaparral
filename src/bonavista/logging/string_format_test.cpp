#include "bonavista/logging/string_format.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(StringFormatTest) {
};

TEST(StringFormatTest, EmptyString) {
  std::string s = StringFormat("");
  EXPECT_EQ(s, "");
}

TEST(StringFormatTest, NoArguments) {
  std::string s = StringFormat("This is a test string");
  EXPECT_EQ(s, "This is a test string");
}

TEST(StringFormatTest, WithArguments) {
  std::string s = StringFormat("This is a test: %d", 5);
  EXPECT_EQ(s, "This is a test: 5");
}
