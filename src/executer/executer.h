#ifndef EXECUTER_EXECUTER_H_
#define EXECUTER_EXECUTER_H_

#include <memory>
#include "executer/variant.h"
#include "third_party/bonavista/src/lexer/token.h"
#include "third_party/bonavista/src/parser/ast_node.h"

using std::shared_ptr;

class Parser;

class Executer {
 public:
  Executer(Parser* parser);
  Executer(const Executer&) = delete;
  Executer& operator=(const Executer&) = delete;
  virtual ~Executer();

  bool Execute(shared_ptr<const Variant>* var);
  template <typename T>
  bool ExecuteT(T* out);
  bool ExecuteAll();

  bool HasInput() const;

  const Token::Position& position() const;
  const std::string& error() const;

 protected:
  virtual bool ExecuteASTNode(const ASTNode* node,
                              shared_ptr<const Variant>* var) = 0;
  template <typename T>
  bool ExecuteASTNodeT(const ASTNode* node, T* out);

  Token::Position position_;
  std::string error_;

 private:
  Parser* parser_;
};

template <typename T>
bool Executer::ExecuteT(T* out) {
  shared_ptr<const Variant> var;
  if (!Execute(&var))
    return false;

  if (!var.get() || !var->Get(out)) {
    error_ = "Unexpected result type";
    return false;
  }

  return true;
}

template <typename T>
bool Executer::ExecuteASTNodeT(const ASTNode* node, T* out) {
  shared_ptr<const Variant> var;
  if (!ExecuteASTNode(node, &var))
    return false;

  if (!var.get() || !var->Get(out)) {
    position_ = node->token()->position();
    error_ = "Unexpected result type";
    return false;
  }

  return true;
}

#endif  // EXECUTER_EXECUTER_H_
