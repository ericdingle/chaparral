#ifndef CHAPARRAL_CALC_CALC_PARSER_H_
#define CHAPARRAL_CALC_CALC_PARSER_H_

#include "chaparral/parser/parser.h"

class CalcParser : public Parser {
 public:
  CalcParser(TokenStream* token_stream);
  virtual ~CalcParser();

  virtual bool Parse(unique_ptr<const ASTNode>* root);

 protected:
  virtual uint GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(unique_ptr<const Token> token,
                                unique_ptr<const ASTNode>* root);
  virtual bool ParseInfixToken(unique_ptr<const Token> token,
                               unique_ptr<const ASTNode> left,
                               unique_ptr<const ASTNode>* root);

 private:
  DISALLOW_COPY_AND_ASSIGN(CalcParser);
};

#endif
