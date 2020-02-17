#include "executer/executer.h"

#include <assert.h>
#include "parser/parser.h"

Executer::Executer(Parser* parser) : parser_(parser) {
}

StatusOr<std::any> Executer::Execute() {
  ASSIGN_OR_RETURN(std::unique_ptr<Node> node, parser_->Parse());
  assert(node);
  return ExecuteNode(node.get());
}

Status Executer::ExecuteAll() {
  while (HasInput()) {
    ASSIGN_OR_RETURN(const std::any any, Execute());
  }
  return Status();
}

bool Executer::HasInput() const {
  return parser_->HasInput();
}
