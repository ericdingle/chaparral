#include "bonavista/logging/string_format.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(StringFormatTest) {
};

TEST(StringFormatTest, EmptyString) {
  std::string s = StringFormat("");
  EXPECT_EQ("", s);
}

TEST(StringFormatTest, NoArguments) {
  std::string s = StringFormat("This is a test string");
  EXPECT_EQ("This is a test string", s);
}

TEST(StringFormatTest, WithArguments) {
  std::string s = StringFormat("This is a test: %d", 5);
  EXPECT_EQ("This is a test: 5", s);
}
