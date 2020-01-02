#ifndef EXECUTER_EXECUTER_TEST_FIXTURE_H_
#define EXECUTER_EXECUTER_TEST_FIXTURE_H_

#include <memory>
#include "executer/any.h"
#include "third_party/bonavista/src/lexer/token_stream.h"
#include "third_party/bonavista/src/util/status_or.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

template <typename LexerT, typename ParserT, typename ExecuterT>
class ExecuterTestFixture : public testing::Test {
 protected:
  void Init(std::string_view input) {
    stream_.reset(new TokenStream(lexer_, input));
    parser_.reset(new ParserT(stream_.get()));
    executer_.reset(new ExecuterT(parser_.get()));
  }

  StatusOr<Any> Execute(std::string_view input) {
    Init(input);
    return executer_->Execute();
  }

  StatusOr<Any> ExecuteAll(std::string_view input) {
    Init(input);
    return executer_->ExecuteAll();
  }

  LexerT lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<ParserT> parser_;
  std::unique_ptr<ExecuterT> executer_;
};

#endif  // EXECUTER_EXECUTER_TEST_FIXTURE_H_
