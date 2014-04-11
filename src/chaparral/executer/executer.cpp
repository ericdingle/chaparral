#include "chaparral/executer/executer.h"

#include "chaparral/parser/parser.h"

Executer::Executer(Parser* parser) : parser_(parser) {
}

Executer::~Executer() {
}

bool Executer::Execute(shared_ptr<const Variant>* var) {
  DCHECK(var);

  std::unique_ptr<const ASTNode> node;
  if (!parser_->Parse(&node)) {
    position_ = parser_->position();
    error_ = parser_->error();
    return false;
  }

  if (!node.get()) {
    var->reset();
    return true;
  }

  return ExecuteASTNode(node.get(), var);
}

bool Executer::ExecuteAll() {
  shared_ptr<const Variant> var;

  while (HasInput()) {
    if (!Execute(&var)) {
      return false;
    }
  }

  return true;
}

bool Executer::HasInput() const {
  return parser_->HasInput();
}

const Token::Position& Executer::position() const {
  return position_;
}

const std::string& Executer::error() const {
  return error_;
}
