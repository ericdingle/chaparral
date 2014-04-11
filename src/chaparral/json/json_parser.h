#ifndef CHAPARRAL_JSON_JSON_PARSER_H_
#define CHAPARRAL_JSON_JSON_PARSER_H_

#include "chaparral/parser/parser.h"

class JsonParser : public Parser {
 public:
  JsonParser(TokenStream* token_stream);
  virtual ~JsonParser();

  virtual bool Parse(unique_ptr<const ASTNode>* root);

 protected:
  virtual bool ParsePrefixToken(unique_ptr<const Token> token,
                                unique_ptr<const ASTNode>* root);

 private:
  bool ParseObject(unique_ptr<const Token> token,
                   unique_ptr<const ASTNode>* root);
  bool ParseArray(unique_ptr<const Token> token,
                  unique_ptr<const ASTNode>* root);

  DISALLOW_COPY_AND_ASSIGN(JsonParser);
};

#endif
