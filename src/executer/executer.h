#ifndef EXECUTER_EXECUTER_H_
#define EXECUTER_EXECUTER_H_

#include <assert.h>
#include <memory>
#include "executer/variant.h"
#include "third_party/bonavista/src/parser/node.h"
#include "third_party/bonavista/src/parser/parser.h"
#include "third_party/bonavista/src/util/status_or.h"

class Executer {
 public:
  Executer(Parser* parser);
  Executer(const Executer&) = delete;
  Executer& operator=(const Executer&) = delete;
  virtual ~Executer() = default;

  StatusOr<std::shared_ptr<Variant>> Execute();
  Status ExecuteAll();

  bool HasInput() const;

 protected:
  virtual StatusOr<std::shared_ptr<Variant>> ExecuteNode(const Node* node) = 0;
  template <typename T>
  StatusOr<T> ExecuteNodeT(const Node* node);

 private:
  Parser* parser_;
};

template <typename T>
StatusOr<T> Executer::ExecuteNodeT(const Node* node) {
  ASSIGN_OR_RETURN(std::shared_ptr<const Variant> var, ExecuteNode(node));
  assert(var);

  T t;
  if (!var->Get(&t)) {
    return Status(std::string("Expected type: ") + typeid(t).name(),
                  node->token().line(), node->token().column());
  }
  return t;
}

#endif  // EXECUTER_EXECUTER_H_
