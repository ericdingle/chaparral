#include "calc/calc_parser.h"

#include <vector>
#include "calc/calc_lexer.h"
#include "parser/ast_node.h"

CalcParser::CalcParser(TokenStream* token_stream) : Parser(token_stream) {
}

CalcParser::~CalcParser() {
}

bool CalcParser::Parse(std::unique_ptr<const ASTNode>* root) {
  std::unique_ptr<const ASTNode> node;
  if (!Parser::Parse(&node))
    return false;
  if (!node.get()) {
    error_ = "Input is empty.";
    return false;
  }

  std::unique_ptr<const ASTNode> dummy;
  if (!Parser::Parse(&dummy) || dummy.get()) {
    error_ = "Encountered more than one expression.";
    return false;
  }

  *root = std::move(node);
  return true;
}

int CalcParser::GetBindingPower(int type) const {
  switch (type) {
    case CalcLexer::TYPE_ASTERISK:
    case CalcLexer::TYPE_SLASH:
      return 20;
    case CalcLexer::TYPE_MINUS:
    case CalcLexer::TYPE_PLUS:
      return 10;
    default:
      return 0;
  }
}

bool CalcParser::ParsePrefixToken(std::unique_ptr<const Token> token,
                                  std::unique_ptr<const ASTNode>* root) {
  if (token->IsType(CalcLexer::TYPE_LEFT_PARENTHESIS)) {
    std::unique_ptr<const ASTNode> node;
    if (!ParseExpression(0, &node))
      return false;

    if (!ConsumeToken(CalcLexer::TYPE_RIGHT_PARENTHESIS))
      return false;

    *root = std::move(node);
    return true;
  }

  if (token->IsType(CalcLexer::TYPE_NUMBER)) {
    root->reset(new ASTNode(std::move(token)));
    return true;
  }

  position_ = token->position();
  error_ = "Unexpected token: " + token->value();
  return false;
}

bool CalcParser::ParseInfixToken(std::unique_ptr<const Token> token,
                                 std::unique_ptr<const ASTNode> left,
                                 std::unique_ptr<const ASTNode>* root) {
  if (token->IsType(CalcLexer::TYPE_ASTERISK) ||
      token->IsType(CalcLexer::TYPE_MINUS) ||
      token->IsType(CalcLexer::TYPE_PLUS) ||
      token->IsType(CalcLexer::TYPE_SLASH)) {
    std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));
    node->AddChild(std::move(left));

    std::unique_ptr<const ASTNode> right;
    if (!ParseExpression(GetBindingPower(node->token()->type()), &right))
      return false;
    node->AddChild(std::move(right));

    *root = std::move(node);
    return true;
  }

  position_ = token->position();
  error_ = "Unexpected token: " + token->value();
  return false;
}
