#include "chaparral/parser/ast_node.h"

#include "chaparral/lexer/token.h"

ASTNode::ASTNode(unique_ptr<const Token> token) : token_(std::move(token)) {
  DCHECK(token_.get());
}

ASTNode::~ASTNode() {
}

const Token* ASTNode::token() const {
  return token_.get();
}

void ASTNode::AddChild(unique_ptr<const ASTNode> node) {
  DCHECK(node.get());
  children_.push_back(std::move(node));
}

const vector<unique_ptr<const ASTNode> >& ASTNode::children() const {
  return children_;
}
