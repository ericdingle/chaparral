#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "third_party/bonavista/src/lexer/token_test_macros.h"
#include "third_party/bonavista/src/parser/parser_test_fixture.h"
#include "third_party/bonavista/src/util/status_test_macros.h"

class CalcParserTest : public ParserTestFixture<CalcLexer, CalcParser> {
};

TEST_F(CalcParserTest, ParseEmpty) {
  EXPECT_STATUS(Parse("").status(), "Unexpected token: (end of input)", 1, 1);
}

TEST_F(CalcParserTest, ParseMultiple) {
  EXPECT_STATUS(Parse("1 2").status(), "Unexpected token: 2", 1, 3);
}

TEST_F(CalcParserTest, ParseUnknown) {
  EXPECT_STATUS(Parse("b").status(), "Unexpected character: b", 1, 1);
}

TEST_F(CalcParserTest, ParseNumber) {
  EXPECT_TOKEN(Parse("1").value()->token(), CalcLexer::TYPE_NUMBER, "1", 1, 1);
}

TEST_F(CalcParserTest, ParseParenthesis) {
  EXPECT_TOKEN(Parse("(1)").value()->token(), CalcLexer::TYPE_NUMBER, "1", 1, 2);
}

TEST_F(CalcParserTest, ParseParenthesisError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"( )", "Unexpected token: )"},
      {"(1", "Unexpected token: (end of input)"},
      {"( +)", "Unexpected token: +"},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_STATUS(Parse(test_case.first).status(), test_case.second, 1, 3);
  }
}

TEST_F(CalcParserTest, ParseOperator) {
  std::pair<const char*, CalcLexer::Type> test_cases[] = {
      {"*", CalcLexer::TYPE_ASTERISK},
      {"-", CalcLexer::TYPE_MINUS},
      {"+", CalcLexer::TYPE_PLUS},
      {"/", CalcLexer::TYPE_SLASH},
      };

  for (const auto& test_case : test_cases) {
    std::string input = std::string("1") + test_case.first + "2";

    auto node = Parse(input.c_str()).value();
    EXPECT_TOKEN(node->token(), test_case.second, test_case.first, 1, 2);
    EXPECT_EQ(2U, node->children().size());

    const auto& children = node->children();
    EXPECT_TOKEN(children[0]->token(), CalcLexer::TYPE_NUMBER, "1", 1, 1);
    EXPECT_TOKEN(children[1]->token(), CalcLexer::TYPE_NUMBER, "2", 1, 3);
  }
}

TEST_F(CalcParserTest, ParseOperatorError) {
  EXPECT_STATUS(Parse("1+*").status(), "Unexpected token: *", 1, 3);
}

TEST_F(CalcParserTest, OperatorPrecedence) {
  std::pair<const char*, CalcLexer::Type> test_cases[] = {
     {"1+2-3", CalcLexer::TYPE_MINUS},
     {"1-2+3", CalcLexer::TYPE_PLUS},
     {"1*2/3", CalcLexer::TYPE_SLASH},
     {"1/2*3", CalcLexer::TYPE_ASTERISK},
     {"1*2+3", CalcLexer::TYPE_PLUS},
     {"1-2/3", CalcLexer::TYPE_MINUS},
     {"1*(2+3)", CalcLexer::TYPE_ASTERISK},
     };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(test_case.second, Parse(test_case.first).value()->token().type());
  }
}
