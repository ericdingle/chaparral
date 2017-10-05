#include <tuple>
#include <vector>
#include "calc/calc_lexer.h"
#include "third_party/googletest/src/googletest/include/gtest/gtest.h"

class CalcLexerTest : public testing::Test {
 protected:
  CalcLexer lexer_;

  std::string input_;
  int type_;
  std::string value_;
  int count_;
  std::string error_;
};

TEST_F(CalcLexerTest, TokenizeUnknown) {
  input_ = ".";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST_F(CalcLexerTest, TokenizeOperator) {
  std::vector<std::tuple<const char*, CalcLexer::Type>> test_cases = {
      std::make_tuple("*", CalcLexer::TYPE_ASTERISK),
      std::make_tuple("(", CalcLexer::TYPE_LEFT_PARENTHESIS),
      std::make_tuple("-", CalcLexer::TYPE_MINUS),
      std::make_tuple("+", CalcLexer::TYPE_PLUS),
      std::make_tuple(")", CalcLexer::TYPE_RIGHT_PARENTHESIS),
      std::make_tuple("/", CalcLexer::TYPE_SLASH),
      };

  for (const std::tuple<const char*, CalcLexer::Type>& test_case : test_cases) {
    input_ = std::get<0>(test_case);
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(std::get<1>(test_case), type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(1, count_);
  }
}

TEST_F(CalcLexerTest, TokenizeNumber) {
  std::vector<const char*> test_cases = {
      "0", "1", "12", "123",
      "0.1", "12.3", "12.34",
      };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(CalcLexer::TYPE_NUMBER, type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST_F(CalcLexerTest, TokenizeNumberError) {
  std::vector<const char*> test_cases = { "01", "1." };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}
