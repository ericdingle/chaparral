#include "chaparral/json/json_parser.h"

#include <vector>
#include "bonavista/logging/string_format.h"
#include "chaparral/json/json_lexer.h"
#include "chaparral/parser/ast_node.h"

JsonParser::JsonParser(TokenStream* token_stream) : Parser(token_stream) {
}

JsonParser::~JsonParser() {
}

bool JsonParser::Parse(unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  unique_ptr<const ASTNode> node;
  if (!Parser::Parse(&node))
    return false;
  if (!node.get()) {
    error_ = "Input is empty.";
    return false;
  }

  unique_ptr<const ASTNode> dummy;
  if (!Parser::Parse(&dummy) || dummy.get()) {
    error_ = "Encountered more than one expression.";
    return false;
  }

  *root = std::move(node);
  return true;
}

bool JsonParser::ParsePrefixToken(unique_ptr<const Token> token,
                                  unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  if (token->IsType(JsonLexer::TYPE_LEFT_BRACE))
    return ParseObject(std::move(token), root);
  else if (token->IsType(JsonLexer::TYPE_LEFT_BRACKET))
    return ParseArray(std::move(token), root);

  if (token->IsType(JsonLexer::TYPE_FALSE) ||
      token->IsType(JsonLexer::TYPE_NULL) ||
      token->IsType(JsonLexer::TYPE_NUMBER) ||
      token->IsType(JsonLexer::TYPE_STRING) ||
      token->IsType(JsonLexer::TYPE_TRUE)) {
    root->reset(new ASTNode(std::move(token)));
    return true;
  }

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}

bool JsonParser::ParseObject(unique_ptr<const Token> token,
                             unique_ptr<const ASTNode>* root) {
  // Implements:
  //  object -> '{' pairs '}'
  //  pair -> string ':' value
  //  pairs -> pair more_pairs | E
  //  more_pairs -> ',' pair more_pairs | E
  DCHECK(root);

  unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACE)) {
    while (true) {
      unique_ptr<const ASTNode> key;
      if (!ParseExpression(0, &key))
        return false;

      if (!key->token()->IsType(JsonLexer::TYPE_STRING)) {
        position_ = key->token()->position();
        error_ = StringFormat("Expecting string but found %s",
                              key->token()->value().c_str());
        return false;
      }

      node->AddChild(std::move(key));

      if (!ConsumeToken(JsonLexer::TYPE_COLON))
        return false;

      unique_ptr<const ASTNode> value;
      if (!ParseExpression(0, &value))
        return false;

      node->AddChild(std::move(value));

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;

      CHECK(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  if (!ConsumeToken(JsonLexer::TYPE_RIGHT_BRACE))
    return false;

  *root = std::move(node);
  return true;
}

bool JsonParser::ParseArray(unique_ptr<const Token> token,
                            unique_ptr<const ASTNode>* root) {
  // Implements:
  //   array -> '[' values ']'
  //   values -> value more_values | E
  //   more_values -> ',' value more_values | E
  DCHECK(root);

  unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACKET)) {
    while (true) {
      unique_ptr<const ASTNode> value;
      if (!ParseExpression(0, &value))
        return false;

      node->AddChild(std::move(value));

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;

      CHECK(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  if (!ConsumeToken(JsonLexer::TYPE_RIGHT_BRACKET))
    return false;

  *root = std::move(node);
  return true;
}
