#include "calc/calc_lexer.h"

CalcLexer::CalcLexer() {
}

CalcLexer::~CalcLexer() {
}

bool CalcLexer::GetToken(const std::string& input,
                         int index,
                         int* type,
                         std::string* value,
                         int* count,
                         std::string* error) const {
  const char& c = input[index];

  if (IsDigit(c))
    return GetNumberToken(input, index, type, value, count, error);

  int t = -1;
  if (c == '*')
    t = TYPE_ASTERISK;
  else if (c == '(')
    t = TYPE_LEFT_PARENTHESIS;
  else if (c == '-')
    t = TYPE_MINUS;
  else if (c == '+')
    t = TYPE_PLUS;
  else if (c == ')')
    t = TYPE_RIGHT_PARENTHESIS;
  else if (c == '/')
    t = TYPE_SLASH;

  if (t != -1) {
    *type = t;
    *value = c;
    *count = 1;
    return true;
  }

  *error = std::string("Unrecognized token: %c") + c;
  return false;
}

bool CalcLexer::GetNumberToken(const std::string& input,
                               int index,
                               int* type,
                               std::string* value,
                               int* count,
                               std::string* error) const {
  const int length = input.length();
  const int start = index;

  if (input[index] == '0') {
    ++index;
    if (index < length && IsDigit(input[index])) {
      *error = "Unexpected digit";
      return false;
    }
  } else
    for (; index < length && IsDigit(input[index]); ++index);

  if (index < length && input[index] == '.') {
    ++index;

    if (index == length) {
      *error = "Unexpected end of input";
      return false;
    } else if (!IsDigit(input[index])) {
      *error = "Expecting digit";
      return false;
    }

    for (; index < length && IsDigit(input[index]); ++index);
  }

  *type = TYPE_NUMBER;
  *value = input.substr(start, index - start);
  *count = index - start;
  return true;
}
