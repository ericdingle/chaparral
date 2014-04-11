#include "chaparral/lexer/token_stream.h"

#include <memory>
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/lexer.h"

TEST_CASE(TokenStreamTest) {
};

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_CHAR
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const {
    DCHECK(type);
    DCHECK(value);
    DCHECK(count);
    DCHECK(error);

    *type = TYPE_CHAR;
    *value = input[index];
    *count = 1;
    return true;
  }
};

TEST(TokenStreamTest, GetNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a bc\nde  ");
  EXPECT_TRUE(token_stream.HasInput());

  std::unique_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "a");
  EXPECT_EQ(token->position().line, 1);
  EXPECT_EQ(token->position().column, 1);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "b");
  EXPECT_EQ(token->position().line, 1);
  EXPECT_EQ(token->position().column, 3);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "c");
  EXPECT_EQ(token->position().line, 1);
  EXPECT_EQ(token->position().column, 4);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "d");
  EXPECT_EQ(token->position().line, 2);
  EXPECT_EQ(token->position().column, 1);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "e");
  EXPECT_EQ(token->position().line, 2);
  EXPECT_EQ(token->position().column, 2);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
  EXPECT_EQ(token->value(), "(end of input)");
  EXPECT_EQ(token->position().line, 2);
  EXPECT_EQ(token->position().column, 5);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
  EXPECT_EQ(token->value(), "(end of input)");
  EXPECT_EQ(token->position().line, 2);
  EXPECT_EQ(token->position().column, 5);
  EXPECT_FALSE(token_stream.HasInput());
}
