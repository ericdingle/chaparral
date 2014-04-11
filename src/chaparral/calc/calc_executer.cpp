#include "chaparral/calc/calc_executer.h"

#include "chaparral/calc/calc_lexer.h"
#include "chaparral/parser/ast_node.h"

CalcExecuter::CalcExecuter(Parser* parser) : Executer(parser) {
}

CalcExecuter::~CalcExecuter() {
}

bool CalcExecuter::ExecuteASTNode(const ASTNode* node,
                                  shared_ptr<const Variant>* var) {
  DCHECK(node);
  DCHECK(var);

  if (node->token()->IsType(CalcLexer::TYPE_ASTERISK) ||
      node->token()->IsType(CalcLexer::TYPE_MINUS) ||
      node->token()->IsType(CalcLexer::TYPE_PLUS) ||
      node->token()->IsType(CalcLexer::TYPE_SLASH)) {
    double left = 0;
    CHECK(ExecuteASTNodeT(node->children()[0].get(), &left));

    double right = 0;
    CHECK(ExecuteASTNodeT(node->children()[1].get(), &right));

    double result = 0;
    if (node->token()->IsType(CalcLexer::TYPE_ASTERISK))
      result = left * right;
    else if (node->token()->IsType(CalcLexer::TYPE_MINUS))
      result = left - right;
    else if (node->token()->IsType(CalcLexer::TYPE_PLUS))
      result = left + right;
    else
      result = left / right;

    var->reset(new Variant(result));
    return true;
  }

  if (node->token()->IsType(CalcLexer::TYPE_NUMBER)) {
    double value = atof(node->token()->value().c_str());
    var->reset(new Variant(value));
    return true;
  }

  // This should never happen.
  CHECK(false);
}
