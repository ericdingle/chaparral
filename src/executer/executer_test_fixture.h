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
  StatusOr<std::shared_ptr<Any>> Execute(const char* input) const {
    LexerT lexer;
    TokenStream stream(&lexer, input);
    ParserT parser(&stream);
    ExecuterT executer(&parser);
    return executer.Execute();
  }

  StatusOr<std::shared_ptr<Any>> ExecuteAll(const char* input) const {
    LexerT lexer;
    TokenStream stream(&lexer, input);
    ParserT parser(&stream);
    ExecuterT executer(&parser);
    return executer.ExecuteAll();
  }
};

#endif  // EXECUTER_EXECUTER_TEST_FIXTURE_H_
