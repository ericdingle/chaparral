#include "calc/calc_executer.h"
#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "executer/executer_test_fixture.h"
#include "third_party/bonavista/src/util/status_test_macros.h"

class CalcExecuterTest : public ExecuterTestFixture<CalcLexer, CalcParser, CalcExecuter> {
};

TEST_F(CalcExecuterTest, ExecuteEmpty) {
  EXPECT_STATUS(Execute("").status(), "Unexpected token: (end of input)", 1, 1);
}

TEST_F(CalcExecuterTest, ExecuteUnknown) {
  EXPECT_STATUS(Execute("b").status(), "Unexpected character: b", 1, 1);
}

TEST_F(CalcExecuterTest, ExecuteNumber) {
  EXPECT_EQ(1.0, std::any_cast<double>(Execute("1").value()));
}

TEST_F(CalcExecuterTest, ExecuteOperator) {
  std::pair<const char*, double> test_cases[] = {
      {"*", 9.0},
      {"-", 0.0},
      {"+", 6.0},
      {"/", 1.0},
      };

  for (const auto& test_case : test_cases) {
    std::string input = std::string("3") + test_case.first + "3";
    EXPECT_EQ(test_case.second, std::any_cast<double>(Execute(input).value()));
  }
}
