#include "chaparral/calc/calc_executer.h"

#include "bonavista/logging/string_format.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/calc/calc_parser.h"
#include "chaparral/lexer/token_stream.h"

TEST_CASE(CalcExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new CalcParser(stream_.get()));
    executer_.reset(new CalcExecuter(parser_.get()));
  }

  CalcLexer lexer_;
  unique_ptr<TokenStream> stream_;
  unique_ptr<Parser> parser_;
  unique_ptr<Executer> executer_;
  shared_ptr<const Variant> var_;
  double d_;
};

TEST(CalcExecuterTest, ExecuteEmpty) {
  Init("");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(CalcExecuterTest, ExecuteUnknown) {
  Init("b");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(CalcExecuterTest, ExecuteNumber) {
  Init("1");
  EXPECT_TRUE(executer_->ExecuteT(&d_));
  EXPECT_EQ(1, d_);
}

TEST(CalcExecuterTest, ExecuteOperator) {
  const char ops[] = { '*', '-', '+', '/' };
  const double results[] = { 9, 0, 6, 1 };

  for (uint i = 0; i < ARRAY_SIZE(ops); ++i) {
    std::string input = StringFormat("3%c3", ops[i]);
    Init(input.c_str());
    EXPECT_TRUE(executer_->ExecuteT(&d_));
    EXPECT_EQ(results[i], d_);
  }
}
