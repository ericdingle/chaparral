#include "calc/calc_lexer.h"

StatusOr<std::unique_ptr<Token>> CalcLexer::GetToken(
    const char* input, int line, int column) const {
  char c = *input;
  if (IsDigit(*input)) {
    return GetNumberToken(input, line, column);
  }

  int t = -1;
  if (c == '*') {
    t = TYPE_ASTERISK;
  } else if (c == '(') {
    t = TYPE_LEFT_PARENTHESIS;
  } else if (c == '-') {
    t = TYPE_MINUS;
  } else if (c == '+') {
    t = TYPE_PLUS;
  } else if (c == ')') {
    t = TYPE_RIGHT_PARENTHESIS;
  } else if (c == '/') {
    t = TYPE_SLASH;
  }

  if (t != -1) {
    return std::unique_ptr<Token>(new Token(t, c, line, column));
  }

  return UnexpectedCharacter(c, line, column);
}

StatusOr<std::unique_ptr<Token>> CalcLexer::GetNumberToken(
    const char* input, int line, int column) const {
  const char* start = input;

  if (*input == '0') {
    ++input;
  } else {
    for (; IsDigit(*input); ++input);
  }

  if (*input == '.') {
    ++input;
    RETURN_IF_ERROR(ExpectDigit(*input, line, column));
    for (; IsDigit(*input); ++input);
  }

  return std::unique_ptr<Token>(new Token(
      TYPE_NUMBER, std::string(start, input - start), line, column));
}
