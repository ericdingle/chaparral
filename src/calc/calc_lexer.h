#ifndef CALC_CALC_LEXER_H_
#define CALC_CALC_LEXER_H_

#include "lexer/lexer.h"

class CalcLexer : public Lexer {
 public:
  CalcLexer();
  CalcLexer(CalcLexer&) = delete;
  CalcLexer& operator=(CalcLexer&) = delete;
  virtual ~CalcLexer();

  enum Type {
    TYPE_ASTERISK,
    TYPE_LEFT_PARENTHESIS,
    TYPE_MINUS,
    TYPE_NUMBER,
    TYPE_PLUS,
    TYPE_RIGHT_PARENTHESIS,
    TYPE_SLASH
  };

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const;

 private:
  bool GetNumberToken(const std::string& input,
                      int index,
                      int* type,
                      std::string* value,
                      int* count,
                      std::string* error) const;
};

#endif  // CALC_CALC_LEXER_H_
