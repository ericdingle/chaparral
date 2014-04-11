#ifndef CHAPARRAL_CALC_CALC_EXECUTER_H_
#define CHAPARRAL_CALC_CALC_EXECUTER_H_

#include "bonavista/base/macros.h"
#include "chaparral/executer/executer.h"

class CalcExecuter : public Executer {
 public:
  CalcExecuter(Parser* parser);
  virtual ~CalcExecuter();

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node,
                              shared_ptr<const Variant>* var);

 private:
  DISALLOW_COPY_AND_ASSIGN(CalcExecuter);
};

#endif
