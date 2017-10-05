#ifndef CALC_CALC_PARSER_H_
#define CALC_CALC_PARSER_H_

#include "parser/parser.h"

class CalcParser : public Parser {
 public:
  CalcParser(TokenStream* token_stream);
  CalcParser(const CalcParser&) = delete;
  CalcParser& operator=(const CalcParser&) = delete;
  virtual ~CalcParser();

  virtual bool Parse(std::unique_ptr<const ASTNode>* root);

 protected:
  virtual int GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode>* root);
  virtual bool ParseInfixToken(std::unique_ptr<const Token> token,
                               std::unique_ptr<const ASTNode> left,
                               std::unique_ptr<const ASTNode>* root);
};

#endif  // CALC_CALC_PARSER_H_
