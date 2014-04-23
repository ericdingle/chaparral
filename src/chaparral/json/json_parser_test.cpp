#include "chaparral/json/json_parser.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/json/json_lexer.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(JsonParserTest) {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new JsonParser(stream_.get()));
  }

  JsonLexer lexer_;
  unique_ptr<TokenStream> stream_;
  unique_ptr<Parser> parser_;
  unique_ptr<const ASTNode> root_;
};

TEST(JsonParserTest, ParseEmpty) {
  Init("");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(JsonParserTest, ParseMultiple) {
  Init("1 2");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(JsonParserTest, ParseUnknown) {
  Init("blah");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(JsonParserTest, ParsePrimitive) {
  Init("false");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_FALSE));

  Init("null");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NULL));

  Init("1");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NUMBER));

  Init("\"test\"");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_STRING));

  Init("true");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_TRUE));
}

TEST(JsonParserTest, ParseObject) {
  Init("{}");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(0u, root_->children().size());

  Init("{\"a\": 1, \"b\": false}");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(4u, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[3]->token()->IsType(JsonLexer::TYPE_FALSE));
}

TEST(JsonParserTest, ParseObjectError) {
  Init("{, \"a\": 1}");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{1: false}");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{\"a\", false}");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{\"a\": }");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{\"a\": false");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(JsonParserTest, ParseArray) {
  Init("[]");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(0u, root_->children().size());

  Init("[1, false, \"test\"]");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(3u, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_FALSE));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
}

TEST(JsonParserTest, ParseArrayError) {
  Init("[, false");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("[1, false, null");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(JsonParserTest, ParseMultipleExpressions) {
  Init("1 false");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("1 blah");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}
