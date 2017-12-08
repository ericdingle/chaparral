#ifndef CALC_CALC_LEXER_H_
#define CALC_CALC_LEXER_H_

#include "third_party/bonavista/src/lexer/lexer.h"

class CalcLexer : public Lexer {
 public:
  CalcLexer() = default;
  CalcLexer(CalcLexer&) = delete;
  CalcLexer& operator=(CalcLexer&) = delete;
  ~CalcLexer() override = default;

  enum Type {
    TYPE_ASTERISK,
    TYPE_LEFT_PARENTHESIS,
    TYPE_MINUS,
    TYPE_NUMBER,
    TYPE_PLUS,
    TYPE_RIGHT_PARENTHESIS,
    TYPE_SLASH
  };

  StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const override;

 private:
  StatusOr<std::unique_ptr<Token>> GetNumberToken(
      const char* input, int line, int column) const;
};

#endif  // CALC_CALC_LEXER_H_
