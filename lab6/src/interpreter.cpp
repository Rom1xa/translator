#include "interpreter.hpp"

#include <iostream>

struct SemanticError {
  std::string message;
};

int Interpreter::execute(const ast::Program &program) {
  try {
    executeStat(*program.stat);
  } catch (const SemanticError &e) {
    std::cout << "Semantic error: " << e.message << "\n";
    return 2;
  }
  return 0;
}

void Interpreter::executeStat(const ast::Stat &stat) {
  if (const auto *ns = dynamic_cast<const ast::NestedStat *>(&stat)) {
    executeForLoop(*ns->forExpr, *ns->body);
    return;
  }
  if (const auto *ls = dynamic_cast<const ast::LeafStat *>(&stat)) {
    executeInit(*ls->forExpr->init);
    int iterations = 0;
    while (evaluateCond(*ls->forExpr->cond)) {
      if (++iterations > MAX_ITERATIONS) {
        throw SemanticError{
            "iteration limit exceeded (possible infinite loop)"};
      }
      executeCmd(*ls->cmd);
      executeAction(*ls->forExpr->actionStr);
    }
    return;
  }
  throw SemanticError{"unknown statement type"};
}

void Interpreter::executeForLoop(const ast::ForExpr &fe,
                                 const ast::Stat &body) {
  executeInit(*fe.init);
  int iterations = 0;
  while (evaluateCond(*fe.cond)) {
    if (++iterations > MAX_ITERATIONS) {
      throw SemanticError{"iteration limit exceeded (possible infinite loop)"};
    }
    executeStat(body);
    executeAction(*fe.actionStr);
  }
}

void Interpreter::executeCmd(const ast::Cmd &cmd) {
  if (!cmd.empty) {
    std::cout << cmd.word << "\n";
  }
}

void Interpreter::executeInit(const ast::Init &init) {
  if (!init.empty) {
    env_[init.lhs] = resolveOperand(*init.rhs);
  }
}

void Interpreter::executeAction(const ast::ActionStr &action) {
  if (action.kind == ast::ActionStr::EMPTY)
    return;

  auto it = env_.find(action.name);
  if (it == env_.end()) {
    throw SemanticError{"undefined variable '" + action.name + "'"};
  }

  if (action.op == "++") {
    ++it->second;
  } else if (action.op == "--") {
    --it->second;
  } else {
    throw SemanticError{"unknown unary operator '" + action.op + "'"};
  }
}

bool Interpreter::evaluateCond(const ast::Cond &cond) const {
  if (cond.empty)
    return true;

  int left = resolveOperand(*cond.left);
  int right = resolveOperand(*cond.right);

  if (cond.op == "<")
    return left < right;
  if (cond.op == "<=")
    return left <= right;
  if (cond.op == ">")
    return left > right;
  if (cond.op == ">=")
    return left >= right;
  if (cond.op == "==")
    return left == right;
  if (cond.op == "!=")
    return left != right;
  if (cond.op == "&&")
    return (left != 0) && (right != 0);
  if (cond.op == "||")
    return (left != 0) || (right != 0);

  throw SemanticError{"unknown comparison operator '" + cond.op + "'"};
}

int Interpreter::resolveOperand(const ast::Operand &op) const {
  if (op.kind == ast::Operand::NUM) {
    return std::stoi(op.value);
  }
  auto it = env_.find(op.value);
  if (it == env_.end()) {
    throw SemanticError{"undefined variable '" + op.value + "'"};
  }
  return it->second;
}
