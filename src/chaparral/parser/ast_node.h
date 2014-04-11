#ifndef CHAPARRAL_PARSER_AST_NODE_H_
#define CHAPARRAL_PARSER_AST_NODE_H_

#include <memory>
#include <vector>
#include "bonavista/base/macros.h"

using std::unique_ptr;
using std::vector;

class Token;

class ASTNode {
 public:
  ASTNode(unique_ptr<const Token> token);
  ~ASTNode();

  const Token* token() const;

  void AddChild(unique_ptr<const ASTNode> node);
  const vector<unique_ptr<const ASTNode> >& children() const;

 private:
  unique_ptr<const Token> token_;
  vector<unique_ptr<const ASTNode> > children_;

  DISALLOW_COPY_AND_ASSIGN(ASTNode);
};

#endif
