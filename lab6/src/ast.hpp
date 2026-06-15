#pragma once

#include <iostream>
#include <memory>
#include <string>

namespace ast {

struct Node {
  virtual ~Node() = default;
  virtual void print(std::ostream &os, int indent = 0) const = 0;
};

inline void printIndent(std::ostream &os, int indent) {
  for (int i = 0; i < indent; ++i)
    os << "  ";
}

struct Operand final : Node {
  enum Kind { NAME, NUM } kind;
  std::string value;
  Operand(Kind k, std::string v) : kind(k), value(std::move(v)) {}
  void print(std::ostream &os, int indent) const override;
};

struct Init final : Node {
  bool empty{true};
  std::string lhs;
  std::unique_ptr<Operand> rhs;
  void print(std::ostream &os, int indent) const override;
};

struct Cond final : Node {
  bool empty{true};
  std::unique_ptr<Operand> left;
  std::string op;
  std::unique_ptr<Operand> right;
  void print(std::ostream &os, int indent) const override;
};

struct ActionStr final : Node {
  enum Kind { PREFIX, POSTFIX, EMPTY } kind{EMPTY};
  std::string op;
  std::string name;
  void print(std::ostream &os, int indent) const override;
};

struct Cmd final : Node {
  bool empty{true};
  std::string word;
  void print(std::ostream &os, int indent) const override;
};

struct ForExpr final : Node {
  std::unique_ptr<Init> init;
  std::unique_ptr<Cond> cond;
  std::unique_ptr<ActionStr> actionStr;
  void print(std::ostream &os, int indent) const override;
};

struct Stat : Node {};

struct NestedStat final : Stat {
  std::unique_ptr<ForExpr> forExpr;
  std::unique_ptr<Stat> body;
  void print(std::ostream &os, int indent) const override;
};

struct LeafStat final : Stat {
  std::unique_ptr<ForExpr> forExpr;
  std::unique_ptr<Cmd> cmd;
  void print(std::ostream &os, int indent) const override;
};

struct Program final : Node {
  std::unique_ptr<Stat> stat;
  void print(std::ostream &os, int indent) const override;
};

} // namespace ast
