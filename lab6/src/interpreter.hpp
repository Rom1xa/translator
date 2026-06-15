#pragma once

#include <map>
#include <string>

#include "ast.hpp"

class Interpreter {
public:
  int execute(const ast::Program &program);

private:
  static constexpr int MAX_ITERATIONS = 10000;

  std::map<std::string, int> env_;

  void executeStat(const ast::Stat &stat);
  void executeForLoop(const ast::ForExpr &fe, const ast::Stat &body);
  void executeCmd(const ast::Cmd &cmd);

  void executeInit(const ast::Init &init);
  void executeAction(const ast::ActionStr &action);

  bool evaluateCond(const ast::Cond &cond) const;
  int resolveOperand(const ast::Operand &op) const;
};
