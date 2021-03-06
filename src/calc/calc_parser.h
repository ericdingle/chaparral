#ifndef CALC_CALC_PARSER_H_
#define CALC_CALC_PARSER_H_

#include "parser/parser.h"

class CalcParser : public Parser {
 public:
  using Parser::Parser;
  CalcParser(const CalcParser&) = delete;
  CalcParser& operator=(const CalcParser&) = delete;
  ~CalcParser() override = default;

  StatusOr<std::unique_ptr<Node>> Parse() override;

 protected:
  int GetBindingPower(int type) const override;
  StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override;
  StatusOr<std::unique_ptr<Node>> ParseInfixToken(
      std::unique_ptr<const Token> token, std::unique_ptr<const Node> left)
      override;
};

#endif  // CALC_CALC_PARSER_H_
