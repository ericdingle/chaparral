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
  EXPECT_EQ("a", token->value());
  EXPECT_EQ(1u, token->position().line);
  EXPECT_EQ(1u, token->position().column);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("b", token->value());
  EXPECT_EQ(1u, token->position().line);
  EXPECT_EQ(3u, token->position().column);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("c", token->value());
  EXPECT_EQ(1u, token->position().line);
  EXPECT_EQ(4u, token->position().column);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("d", token->value());
  EXPECT_EQ(2u, token->position().line);
  EXPECT_EQ(1u, token->position().column);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("e", token->value());
  EXPECT_EQ(2u, token->position().line);
  EXPECT_EQ(2u, token->position().column);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
  EXPECT_EQ("(end of input)", token->value());
  EXPECT_EQ(2u, token->position().line);
  EXPECT_EQ(5u, token->position().column);

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
  EXPECT_EQ("(end of input)", token->value());
  EXPECT_EQ(2u, token->position().line);
  EXPECT_EQ(5u, token->position().column);
  EXPECT_FALSE(token_stream.HasInput());
}
