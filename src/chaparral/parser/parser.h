#ifndef CHAPARRAL_PARSER_PARSER_H_
#define CHAPARRAL_PARSER_PARSER_H_

#include <memory>
#include <string>
#include "bonavista/base/macros.h"
#include "chaparral/lexer/token.h"

using std::string;
using std::unique_ptr;

class ASTNode;
class TokenStream;

class Parser {
 public:
  Parser(TokenStream* token_stream);
  virtual ~Parser();

  virtual bool Parse(unique_ptr<const ASTNode>* root);

  bool HasInput() const;

  const Token::Position& position() const;
  const string& error() const;

 private:
  bool Init();

 protected:
  bool GetNextToken(unique_ptr<const Token>* token);
  bool ParseExpression(uint binding_power, unique_ptr<const ASTNode>* root);
  bool ConsumeToken(int type);

  virtual uint GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(unique_ptr<const Token> token,
                                unique_ptr<const ASTNode>* root) =0;
  virtual bool ParseInfixToken(unique_ptr<const Token> token,
                               unique_ptr<const ASTNode> left,
                               unique_ptr<const ASTNode>* root);

  unique_ptr<const Token> look_ahead_token_;

  Token::Position position_;
  string error_;

 private:
  TokenStream* token_stream_;

  DISALLOW_COPY_AND_ASSIGN(Parser);
};

#endif
