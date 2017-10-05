#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "calc/calc_executer.h"
#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "lexer/token_stream.h"

namespace {
const char* kInputPrefix = "calc > ";
}  // namespace

int main() {
  CalcLexer lexer;

  while (true) {
    printf("%s", kInputPrefix);
    std::string input;
    getline(std::cin, input);
    if (input.empty())
      break;

    TokenStream stream(&lexer, input);
    CalcParser parser(&stream);
    CalcExecuter executer(&parser);
    double result = 0;
    if (!executer.ExecuteT(&result)) {
      int offset = static_cast<int>(strlen(kInputPrefix) + parser.position().column);
      printf("%*s\n", offset, "^");
      printf("Error: %s.\n", parser.error().c_str());
      continue;
    }

    printf("%f\n", result);
  }

  return 0;
}
