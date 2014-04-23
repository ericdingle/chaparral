#include "chaparral/calc/calc_parser.h"

#include "bonavista/logging/string_format.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(CalcParserTest) {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new CalcParser(stream_.get()));
  }

  CalcLexer lexer_;
  unique_ptr<TokenStream> stream_;
  unique_ptr<Parser> parser_;
  unique_ptr<const ASTNode> root_;
};

TEST(CalcParserTest, ParseEmpty) {
  Init("");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, ParseMultiple) {
  Init("1 2");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, ParseUnknown) {
  Init("b");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, ParseNumber) {
  Init("1");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST(CalcParserTest, ParseParenthesis) {
  Init("(1)");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST(CalcParserTest, ParseParenthesisError) {
  const char* inputs[] = { "()", "(1", "(+)" };
  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    Init(inputs[i]);
    EXPECT_FALSE(parser_->Parse(&root_));
    EXPECT_FALSE(parser_->error().empty());
  }
}

TEST(CalcParserTest, ParseOperator) {
  const char ops[] = { '*', '-', '+', '/' };
  const int types[] = { CalcLexer::TYPE_ASTERISK,
                        CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_SLASH };

  for (uint i = 0; i < ARRAY_SIZE(ops); ++i) {
    std::string input = StringFormat("1%c2", ops[i]);
    Init(input.c_str());
    EXPECT_TRUE(parser_->Parse(&root_));

    EXPECT_EQ(types[i], root_->token()->type());
    EXPECT_EQ(2u, root_->children().size());

    EXPECT_EQ(CalcLexer::TYPE_NUMBER, root_->children()[0]->token()->type());
    EXPECT_EQ("1", root_->children()[0]->token()->value());

    EXPECT_EQ(CalcLexer::TYPE_NUMBER, root_->children()[1]->token()->type());
    EXPECT_EQ("2", root_->children()[1]->token()->value());
  }
}

TEST(CalcParserTest, ParseOperatorError) {
  Init("1+*");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, OperatorPrecedence) {
  const char* inputs[] = { "1+2-3",
                           "1-2+3",
                           "1*2/3",
                           "1/2*3",
                           "1*2+3",
                           "1-2/3",
                           "1*(2+3)" };
  const int types[] = { CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_SLASH,
                        CalcLexer::TYPE_ASTERISK,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_ASTERISK };
  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    Init(inputs[i]);
    EXPECT_TRUE(parser_->Parse(&root_));
    EXPECT_EQ(types[i], root_->token()->type());
  }
}
