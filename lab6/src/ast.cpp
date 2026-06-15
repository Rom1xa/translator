#include "ast.hpp"

namespace ast {

void Operand::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  os << (kind == NAME ? "Name(" : "Num(") << value << ")\n";
}

void Init::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  if (empty) {
    os << "Init(empty)\n";
  } else {
    os << "Init(" << lhs << " =)\n";
    rhs->print(os, indent + 1);
  }
}

void Cond::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  if (empty) {
    os << "Cond(empty)\n";
  } else {
    os << "Cond(" << op << ")\n";
    left->print(os, indent + 1);
    right->print(os, indent + 1);
  }
}

void ActionStr::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  if (kind == EMPTY) {
    os << "ActionStr(empty)\n";
  } else {
    const char *k = (kind == PREFIX) ? "prefix" : "postfix";
    os << "ActionStr(" << k << " " << op << " " << name << ")\n";
  }
}

void Cmd::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  if (empty) {
    os << "Cmd(empty)\n";
  } else {
    os << "Cmd(print \"" << word << "\")\n";
  }
}

void ForExpr::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  os << "ForExpr\n";
  init->print(os, indent + 1);
  cond->print(os, indent + 1);
  actionStr->print(os, indent + 1);
}

void NestedStat::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  os << "NestedStat\n";
  forExpr->print(os, indent + 1);
  printIndent(os, indent + 1);
  os << "Body\n";
  body->print(os, indent + 2);
}

void LeafStat::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  os << "LeafStat\n";
  forExpr->print(os, indent + 1);
  cmd->print(os, indent + 1);
}

void Program::print(std::ostream &os, int indent) const {
  printIndent(os, indent);
  os << "Program\n";
  stat->print(os, indent + 1);
}

} // namespace ast
