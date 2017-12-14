#include "executer/any.h"
#include "executer/any_test_macros.h"
#include "executer/executer.h"
#include "executer/executer_test_fixture.h"
#include "third_party/bonavista/src/lexer/lexer.h"
#include "third_party/bonavista/src/parser/node.h"
#include "third_party/bonavista/src/parser/parser.h"
#include "third_party/bonavista/src/util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT
  };

  StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const override {
    if (IsDigit(*input)) {
      return std::unique_ptr<Token>(new Token(TYPE_DIGIT, std::string(1, *input),
                                              line, column));
    }

    return UnexpectedCharacter(*input, line, column);
  }
};

class TestParser : public Parser {
 public:
  using Parser::Parser;

 protected:
  StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override {
    return std::unique_ptr<Node>(new Node(std::move(token)));
  }
};

class TestExecuter : public Executer {
 public:
  using Executer::Executer;
  using Executer::ExecuteNodeT;

 protected:
  StatusOr<Any> ExecuteNode(const Node* node) override {
    int digit = node->token().value()[0] - 0x30;
    if (digit == 9) {
      return Status("No nines!", node->token().line(), node->token().column());
    }

    return Any(digit);
  }
};

class ExecuterTest
    : public ExecuterTestFixture<TestLexer, TestParser, TestExecuter> {
};

TEST_F(ExecuterTest, Empty) {
  EXPECT_STATUS(Execute("").status(), "Unexpected token: (end of input)", 1, 1);
}

TEST_F(ExecuterTest, BadToken) {
  EXPECT_STATUS(Execute("a").status(), "Unexpected character: a", 1, 1);
}

TEST_F(ExecuterTest, Execute) {
  EXPECT_ANY(Execute("5").value(), int, 5);
}

TEST_F(ExecuterTest, ExecuteError) {
  EXPECT_STATUS(Execute("9").status(), "No nines!", 1, 1);
}

TEST_F(ExecuterTest, ExecuteAll) {
  EXPECT_OK(ExecuteAll("1 2 3 4 5"));
}

TEST_F(ExecuterTest, ExecuteAllError) {
  EXPECT_STATUS(ExecuteAll("1 3 5 7 9").status(), "No nines!", 1, 9);
}

TEST_F(ExecuterTest, ExecuteNodeT) {
  TestExecuter executer(nullptr);

  Node node(std::unique_ptr<Token>(new Token(TestLexer::TYPE_DIGIT, "1", 1, 1)));
  EXPECT_EQ(1, executer.ExecuteNodeT<int>(&node).value());

  EXPECT_STATUS(executer.ExecuteNodeT<double>(&node).status(),
                "Expected type: d", 1, 1);
}
