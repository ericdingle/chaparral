#include "chaparral/parser/parser.h"

#include "bonavista/logging/string_format.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT,
    TYPE_PLUS
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const {
    const char& c = input[index];
    if (c == '+') {
      *type = TYPE_PLUS;
    } else if (IsDigit(c)) {
      *type = TYPE_DIGIT;
    } else {
      *error = StringFormat("Unexpected token: %c", c);
      return false;
    }

    *value = c;
    *count = 1;
    return true;
  }
};

class TestParser : public Parser {
 public:
  TestParser(TokenStream* token_stream) : Parser(token_stream) {
  }

 protected:
  virtual uint GetBindingPower(int type) const {
    if (type == TestLexer::TYPE_PLUS)
        return 10;

    return 0;
  }

  virtual bool ParsePrefixToken(unique_ptr<const Token> token,
                                unique_ptr<const ASTNode>* root) {
    DCHECK(root);

    if (token->IsType(TestLexer::TYPE_DIGIT)) {
      root->reset(new ASTNode(std::move(token)));

      // Weird behavior only for the test.
      if (root->get()->token()->value() == "0")
        return ConsumeToken(TestLexer::TYPE_DIGIT);

      return true;
    }

    position_ = token->position();
    error_ = StringFormat("Unexpected token: %s", token->value().c_str());
    return false;
  }

  virtual bool ParseInfixToken(unique_ptr<const Token> token,
                               unique_ptr<const ASTNode> left,
                               unique_ptr<const ASTNode>* root) {
    DCHECK(root);

    if (token->IsType(TestLexer::TYPE_PLUS)) {
      unique_ptr<ASTNode> node(new ASTNode(std::move(token)));
      node->AddChild(std::move(left));

      unique_ptr<const ASTNode> right;
      if (!ParseExpression(10, &right))
        return false;
      node->AddChild(std::move(right));

      *root = std::move(node);
      return true;
    }

    position_ = token->position();
    error_ = StringFormat("Unexpected token: %s", token->value().c_str());
    return false;
  }
};

TEST_CASE(ParserTest) {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new TestParser(stream_.get()));
  }

  TestLexer lexer_;
  unique_ptr<TokenStream> stream_;
  unique_ptr<Parser> parser_;
  unique_ptr<const ASTNode> root_;
};

TEST(ParserTest, Empty) {
  Init("");
  EXPECT_FALSE(parser_->HasInput());
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NULL(root_.get());
  EXPECT_FALSE(parser_->HasInput());
}

TEST(ParserTest, BadToken) {
  Init("a");
  EXPECT_TRUE(parser_->HasInput());
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
  EXPECT_TRUE(parser_->HasInput());
}

TEST(ParserTest, Prefix) {
  Init("1");
  EXPECT_TRUE(parser_->HasInput());
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NOT_NULL(root_.get());
  EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(root_->children().size(), 0);

  EXPECT_FALSE(parser_->HasInput());
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NULL(root_.get());
}

TEST(ParserTest, PrefixError) {
  Init("+");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(ParserTest, Infix) {
  Init("1+2");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NOT_NULL(root_.get());
  EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_PLUS));
  EXPECT_EQ(root_->children().size(), 2);

  const ASTNode* child = root_->children()[0].get();
  EXPECT_TRUE(child->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(child->token()->value(), "1");
  EXPECT_EQ(child->children().size(), 0);

  child = root_->children()[1].get();
  EXPECT_TRUE(child->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(child->token()->value(), "2");
  EXPECT_EQ(child->children().size(), 0);

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NULL(root_.get());
}

TEST(ParserTest, InfixError) {
  Init("1+");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(ParserTest, ConsumeToken) {
  Init("01");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NOT_NULL(root_.get());
  EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(root_->token()->value(), "0");
  EXPECT_EQ(root_->children().size(), 0);

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NULL(root_.get());
}

TEST(ParserTest, ConsumeTokenError) {
  Init("0");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_EQ(parser_->position().line, 1);
  EXPECT_EQ(parser_->position().column, 2);
  EXPECT_FALSE(parser_->error().empty());
}

TEST(ParserTest, ParseMultiple) {
  Init("1 1 1");

  for (uint i = 0; i < 3; ++i) {
    EXPECT_TRUE(parser_->Parse(&root_));
    EXPECT_NOT_NULL(root_.get());
    EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_DIGIT));
    EXPECT_EQ(root_->children().size(), 0);
  }

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NULL(root_.get());

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NULL(root_.get());
}
