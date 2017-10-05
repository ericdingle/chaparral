#include <vector>
#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "lexer/token_stream.h"
#include "parser/ast_node.h"
#include "third_party/googletest/src/googletest/include/gtest/gtest.h"

class CalcParserTest : public testing::Test {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new CalcParser(stream_.get()));
  }

  CalcLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<const ASTNode> root_;
};

TEST_F(CalcParserTest, ParseEmpty) {
  Init("");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(CalcParserTest, ParseMultiple) {
  Init("1 2");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(CalcParserTest, ParseUnknown) {
  Init("b");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(CalcParserTest, ParseNumber) {
  Init("1");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST_F(CalcParserTest, ParseParenthesis) {
  Init("(1)");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST_F(CalcParserTest, ParseParenthesisError) {
  std::vector<const char*> test_cases = { "()", "(1", "(+)" };
  for (auto test_case : test_cases) {
    Init(test_case);
    EXPECT_FALSE(parser_->Parse(&root_));
    EXPECT_FALSE(parser_->error().empty());
  }
}

TEST_F(CalcParserTest, ParseOperator) {
  std::vector<std::tuple<const char*, CalcLexer::Type>> test_cases = {
      std::make_tuple("*", CalcLexer::TYPE_ASTERISK),
      std::make_tuple("-", CalcLexer::TYPE_MINUS),
      std::make_tuple("+", CalcLexer::TYPE_PLUS),
      std::make_tuple("/", CalcLexer::TYPE_SLASH),
      };

  for (const auto& test_case : test_cases) {
    std::string input = std::string("1") + std::get<0>(test_case) + "2";
    Init(input.c_str());
    EXPECT_TRUE(parser_->Parse(&root_));

    EXPECT_EQ(std::get<1>(test_case), root_->token()->type());
    EXPECT_EQ(2, root_->children().size());

    EXPECT_EQ(CalcLexer::TYPE_NUMBER, root_->children()[0]->token()->type());
    EXPECT_EQ("1", root_->children()[0]->token()->value());

    EXPECT_EQ(CalcLexer::TYPE_NUMBER, root_->children()[1]->token()->type());
    EXPECT_EQ("2", root_->children()[1]->token()->value());
  }
}

TEST_F(CalcParserTest, ParseOperatorError) {
  Init("1+*");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(CalcParserTest, OperatorPrecedence) {
  std::vector<std::tuple<const char*, CalcLexer::Type>> test_cases = {
     std::make_tuple("1+2-3", CalcLexer::TYPE_MINUS),
     std::make_tuple("1-2+3", CalcLexer::TYPE_PLUS),
     std::make_tuple("1*2/3", CalcLexer::TYPE_SLASH),
     std::make_tuple("1/2*3", CalcLexer::TYPE_ASTERISK),
     std::make_tuple("1*2+3", CalcLexer::TYPE_PLUS),
     std::make_tuple("1-2/3", CalcLexer::TYPE_MINUS),
     std::make_tuple("1*(2+3)", CalcLexer::TYPE_ASTERISK),
     };

  for (const auto& test_case : test_cases) {
    Init(std::get<0>(test_case));
    EXPECT_TRUE(parser_->Parse(&root_));
    EXPECT_EQ(std::get<1>(test_case), root_->token()->type());
  }
}
