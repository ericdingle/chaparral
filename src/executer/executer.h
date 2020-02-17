#ifndef EXECUTER_EXECUTER_H_
#define EXECUTER_EXECUTER_H_

#include <any>
#include "third_party/bonavista/src/parser/node.h"
#include "third_party/bonavista/src/parser/parser.h"
#include "third_party/bonavista/src/util/status_or.h"

class Executer {
 public:
  Executer(Parser* parser);
  Executer(const Executer&) = delete;
  Executer& operator=(const Executer&) = delete;
  virtual ~Executer() = default;

  StatusOr<std::any> Execute();
  Status ExecuteAll();

  bool HasInput() const;

 protected:
  virtual StatusOr<std::any> ExecuteNode(const Node& node) = 0;
  template <typename T>
  StatusOr<T> ExecuteNodeT(const Node& node);

 private:
  Parser* parser_;
};

template <typename T>
StatusOr<T> Executer::ExecuteNodeT(const Node& node) {
  ASSIGN_OR_RETURN(const std::any any, ExecuteNode(node));

  if (any.type() != typeid(T)) {
    return Status(std::string("Expected type: ") + typeid(T).name(),
                  node.token().line(), node.token().column());
  }

  return std::any_cast<T>(any);
}

#endif  // EXECUTER_EXECUTER_H_
