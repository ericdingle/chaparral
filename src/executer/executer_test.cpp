#include "executer/executer.h"

#include "executer/executer.h"
#include "executer/variant.h"
#include "lexer/lexer.h"
#include "lexer/token_stream.h"
#include "parser/ast_node.h"
#include "parser/parser.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT
  };

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const {
    if (!IsDigit(input[index])) {
      *error = "Expecting digit";
      return false;
    }

    *type = TYPE_DIGIT;
    *value = input[index];
    *count = 1;
    return true;
  }
};

class TestParser : public Parser {
 public:
  TestParser(TokenStream* stream) : Parser(stream) {}

 protected:
  virtual bool ParsePrefixToken(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode>* root) {
    root->reset(new ASTNode(std::move(token)));
    return true;
  }
};

class TestExecuter : public Executer {
 public:
  TestExecuter(Parser* parser) : Executer(parser) {}

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node,
                              std::shared_ptr<const Variant>* var) {
    int digit = node->token()->value()[0] - 0x30;
    if (digit == 9) {
      position_ = node->token()->position();
      error_ = "No nines!";
      return false;
    }

    var->reset(new Variant(digit));
    return true;
  }
};

class ExecuterTest : public testing::Test {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new TestParser(stream_.get()));
    executer_.reset(new TestExecuter(parser_.get()));
  }

  TestLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<Executer> executer_;
  std::shared_ptr<const Variant> var_;
};

TEST_F(ExecuterTest, Empty) {
  Init("");
  EXPECT_FALSE(executer_->HasInput());
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_EQ(nullptr, var_.get());
  EXPECT_FALSE(executer_->HasInput());
}

TEST_F(ExecuterTest, BadToken) {
  Init("a");
  EXPECT_TRUE(executer_->HasInput());
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
  EXPECT_TRUE(executer_->HasInput());
}

TEST_F(ExecuterTest, Execute) {
  Init("5");
  EXPECT_TRUE(executer_->HasInput());
  EXPECT_TRUE(executer_->Execute(&var_));
  int i = 0;
  EXPECT_TRUE(var_->Get(&i));
  EXPECT_EQ(5, i);
  EXPECT_FALSE(executer_->HasInput());
}

TEST_F(ExecuterTest, ExecuteError) {
  Init("9");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST_F(ExecuterTest, ExecuteT) {
  Init("5");
  int i = 0;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(5, i);
}

TEST_F(ExecuterTest, ExecuteTError) {
  int i = 0;
  Init("");
  EXPECT_FALSE(executer_->ExecuteT(&i));
  EXPECT_FALSE(executer_->error().empty());

  Init("9");
  EXPECT_FALSE(executer_->ExecuteT(&i));
  EXPECT_FALSE(executer_->error().empty());

  double d = 0;
  Init("5");
  EXPECT_FALSE(executer_->ExecuteT(&d));
  EXPECT_FALSE(executer_->error().empty());
}

TEST_F(ExecuterTest, ExecuteAll) {
  Init("1 2 3 4 5");
  EXPECT_TRUE(executer_->ExecuteAll());
  std::unique_ptr<const Token> token;
  EXPECT_TRUE(stream_->GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
}

TEST_F(ExecuterTest, ExecuteAllError) {
  Init("1 3 5 7 9");
  EXPECT_FALSE(executer_->ExecuteAll());
}
