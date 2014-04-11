#include "chaparral/parser/parser.h"

#include "bonavista/logging/string_format.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

Parser::Parser(TokenStream* token_stream) : token_stream_(token_stream) {
  DCHECK(token_stream);
}

Parser::~Parser() {
}

bool Parser::Parse(unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  if (!Init())
    return false;

  // Check for end of input.
  if (look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT)) {
    root->reset(NULL);
    return true;
  }

  // Parse expression.
  return ParseExpression(0, root);
}

bool Parser::HasInput() const {
  return look_ahead_token_.get() ?
      !look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT) :
      token_stream_->HasInput();
}

const Token::Position& Parser::position() const {
  return position_;
}

const std::string& Parser::error() const {
  return error_;
}

bool Parser::Init() {
  if (look_ahead_token_.get())
    return true;

  // Advance the look ahead token to the first token.
  unique_ptr<const Token> token;
  return GetNextToken(&token);
}

bool Parser::GetNextToken(unique_ptr<const Token>* token) {
  DCHECK(token);

  *token = std::move(look_ahead_token_);

  if (!token_stream_->GetNextToken(&look_ahead_token_)) {
    position_ = token_stream_->position();
    error_ = token_stream_->error();
    return false;
  }
  DCHECK(look_ahead_token_.get());

  return true;
}

bool Parser::ParseExpression(uint binding_power,
                             unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  unique_ptr<const Token> token;
  if (!GetNextToken(&token))
    return false;

  unique_ptr<const ASTNode> left;
  if (!ParsePrefixToken(std::move(token), &left))
    return false;
  DCHECK(left.get());

  while (binding_power < GetBindingPower(look_ahead_token_->type())) {
    if (!GetNextToken(&token))
      return false;

    if (!ParseInfixToken(std::move(token), std::move(left), &left))
      return false;
    DCHECK(left.get());
  }

  *root = std::move(left);
  return true;
}

bool Parser::ConsumeToken(int type) {
  unique_ptr<const Token> token;
  if (!GetNextToken(&token))
    return false;

  if (!token->IsType(type)) {
    position_ = token->position();
    error_ = StringFormat("Unexpected token: %s", token->value().c_str());
    return false;
  }

  return true;
}

uint Parser::GetBindingPower(int type) const {
  return 0;
}

bool Parser::ParseInfixToken(unique_ptr<const Token> token,
                             unique_ptr<const ASTNode> left,
                             unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}
