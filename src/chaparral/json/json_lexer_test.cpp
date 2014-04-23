#include "chaparral/json/json_lexer.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(JsonLexerTest) {
 protected:
  JsonLexer lexer_;

  std::string input_;
  int type_;
  std::string value_;
  uint count_;
  std::string error_;
};

TEST(JsonLexerTest, TokenizeUnknown) {
  input_ = "blah";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST(JsonLexerTest, TokenizeOperators) {
  const char* inputs[] = { ":", ",", "{", "}", "[", "]" };
  const int types[] = { JsonLexer::TYPE_COLON,
                        JsonLexer::TYPE_COMMA,
                        JsonLexer::TYPE_LEFT_BRACE,
                        JsonLexer::TYPE_RIGHT_BRACE,
                        JsonLexer::TYPE_LEFT_BRACKET,
                        JsonLexer::TYPE_RIGHT_BRACKET };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(types[i], type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(1u, count_);
  }

  input_ = ".";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST(JsonLexerTest, TokenizeKeyword) {
  const char* inputs[] = { "false", "null", "true" };
  const int types[] = { JsonLexer::TYPE_FALSE,
                        JsonLexer::TYPE_NULL,
                        JsonLexer::TYPE_TRUE };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(types[i], type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST(JsonLexerTest, TokenizeNumber) {
  const char* inputs[] = {
    "0", "-0", "1", "-1", "12", "123",
    "0.1", "-0.1", "12.3", "12.34",
    "5e3", "32E-1", "120e+012"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(JsonLexer::TYPE_NUMBER, type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST(JsonLexerTest, TokenizeNumberError) {
  const char* inputs[] = { "-", "01", "1.", "23e", "35E+" };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}

TEST(JsonLexerTest, TokenizeString) {
  const char* inputs[] = {
    "\"test\"",
    "\"asdf jkl;\"",
    "\"test\\b\"",
    "\"test\\f\"",
    "\"test\\n\"",
    "\"test\\r\"",
    "\"test\\t\"",
    "\"test\\u1234\"",
    "\"test\\\\\"",
    "\"test\\/\"",
    "\"test\\\"\"",
  };
  const char* values[] = {
    "test",
    "asdf jkl;",
    "test\b",
    "test\f",
    "test\n",
    "test\r",
    "test\t",
    "test?",
    "test\\",
    "test/",
    "test\"",
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(JsonLexer::TYPE_STRING, type_);
    EXPECT_EQ(values[i], value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST(JsonLexerTest, TokenizeStringError) {
  const char* inputs[] = {
    "\"test",
    "\"test\n",
    "\"test\\",
    "\"test\\a",
    "\"test\\u12",
    "\"test\\u123e"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}
