#ifndef CALC_CALC_EXECUTER_H_
#define CALC_CALC_EXECUTER_H_

#include "executer/executer.h"

class CalcExecuter : public Executer {
 public:
  CalcExecuter(Parser* parser);
  CalcExecuter(const CalcExecuter&) = delete;
  CalcExecuter& operator=(const CalcExecuter&) = delete;
  virtual ~CalcExecuter();

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node,
                              std::shared_ptr<const Variant>* var);
};

#endif  // CALC_CALC_EXECUTER_H_
