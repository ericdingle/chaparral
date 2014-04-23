#include "chaparral/executer/executer.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/executer/executer.h"
#include "chaparral/executer/variant.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"
#include "chaparral/parser/parser.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const {
    DCHECK(type);
    DCHECK(value);
    DCHECK(count);
    DCHECK(error);

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
    DCHECK(root);
    root->reset(new ASTNode(std::move(token)));
    return true;
  }
};

class TestExecuter : public Executer {
 public:
  TestExecuter(Parser* parser) : Executer(parser) {}

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node,
                              shared_ptr<const Variant>* var) {
    DCHECK(node);
    DCHECK(var);

    uint digit = node->token()->value()[0] - 0x30;
    if (digit == 9) {
      position_ = node->token()->position();
      error_ = "No nines!";
      return false;
    }

    var->reset(new Variant(digit));
    return true;
  }
};

TEST_CASE(ExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new TestParser(stream_.get()));
    executer_.reset(new TestExecuter(parser_.get()));
  }

  TestLexer lexer_;
  unique_ptr<TokenStream> stream_;
  unique_ptr<Parser> parser_;
  unique_ptr<Executer> executer_;
  shared_ptr<const Variant> var_;
};

TEST(ExecuterTest, Empty) {
  Init("");
  EXPECT_FALSE(executer_->HasInput());
  EXPECT_TRUE(executer_->Execute(&var_));
  EXPECT_NULL(var_.get());
  EXPECT_FALSE(executer_->HasInput());
}

TEST(ExecuterTest, BadToken) {
  Init("a");
  EXPECT_TRUE(executer_->HasInput());
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
  EXPECT_TRUE(executer_->HasInput());
}

TEST(ExecuterTest, Execute) {
  Init("5");
  EXPECT_TRUE(executer_->HasInput());
  EXPECT_TRUE(executer_->Execute(&var_));
  uint i = 0;
  EXPECT_TRUE(var_->Get(&i));
  EXPECT_EQ(5u, i);
  EXPECT_FALSE(executer_->HasInput());
}

TEST(ExecuterTest, ExecuteError) {
  Init("9");
  EXPECT_FALSE(executer_->Execute(&var_));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(ExecuterTest, ExecuteT) {
  Init("5");
  uint i = 0;
  EXPECT_TRUE(executer_->ExecuteT(&i));
  EXPECT_EQ(5u, i);
}

TEST(ExecuterTest, ExecuteTError) {
  uint i = 0;
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

TEST(ExecuterTest, ExecuteAll) {
  Init("1 2 3 4 5");
  EXPECT_TRUE(executer_->ExecuteAll());
  unique_ptr<const Token> token;
  EXPECT_TRUE(stream_->GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
}

TEST(ExecuterTest, ExecuteAllError) {
  Init("1 3 5 7 9");
  EXPECT_FALSE(executer_->ExecuteAll());
}
