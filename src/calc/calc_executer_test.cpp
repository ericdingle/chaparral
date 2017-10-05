#include <tuple>
#include <vector>
#include "calc/calc_executer.h"
#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "lexer/token_stream.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class CalcExecuterTest : public testing::Test {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new CalcParser(stream_.get()));
    executer_.reset(new CalcExecuter(parser_.get()));
  }

  CalcLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<Executer> executer_;
  shared_ptr<const Variant> var_;
  double d_;
};

TEST_F(CalcExecuterTest, ExecuteEmpty) {
  Init("");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST_F(CalcExecuterTest, ExecuteUnknown) {
  Init("b");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST_F(CalcExecuterTest, ExecuteNumber) {
  Init("1");
  EXPECT_TRUE(executer_->ExecuteT(&d_));
  EXPECT_EQ(1, d_);
}

TEST_F(CalcExecuterTest, ExecuteOperator) {
  std::vector<std::tuple<const char*, double>> test_cases = {
      std::make_tuple("*", 9),
      std::make_tuple("-", 0),
      std::make_tuple("+", 6),
      std::make_tuple("/", 1),
      };

  for (const std::tuple<const char*, double>& test_case : test_cases) {
    std::string input = std::string("3") + std::get<0>(test_case) + "3";
    Init(input.c_str());
    EXPECT_TRUE(executer_->ExecuteT(&d_));
    EXPECT_EQ(std::get<1>(test_case), d_);
  }
}
