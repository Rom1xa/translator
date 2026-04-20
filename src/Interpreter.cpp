#include "Interpreter.hpp"
#include "Definitions.hpp"
#include "Errors.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>

Interpreter::Interpreter() : _stream(std::cout) {}

Interpreter::Interpreter(std::ostream &stream) : _stream(stream) {}

std::unordered_map<std::string, std::function<bool(uint64_t, uint64_t)>>
    Interpreter::comp{
        {">", [](auto a, auto b) { return a > b; }},
        {"<", [](auto a, auto b) { return a < b; }},
        {">=", [](auto a, auto b) { return a >= b; }},
        {"<=", [](auto a, auto b) { return a <= b; }},
        {"==", [](auto a, auto b) { return a == b; }},
        {"!=", [](auto a, auto b) { return a != b; }},
        {"&&", [](auto a, auto b) { return a && b; }},
        {"||", [](auto a, auto b) { return a || b; }},
        {"", [](auto a, auto b) { return true; }},
    };

void checks(Token t, size_t scope) {
  if (t.first == TokenType::NAME) {
    if (symbolTable.count(t.second) == 0) // Неинициализированная переменная
    {
      throw SemanticError(t.second + std::string(" is uninitialized"));
    }

    if (symbolTable[t.second].scope > scope) {
      throw SemanticError(t.second + std::string(" is used out of its scope"));
    }
  }
}

template <typename T> void erase_idx(std::vector<T> &arr, std::size_t s) {
  if (arr.size() > s) {
    auto it = arr.begin() + s;
    arr.erase(it);
  }
}

std::unique_ptr<ParseNode>
Interpreter::shrink(std::unique_ptr<ParseNode> tree) {
  auto shrink_for = [](std::unique_ptr<ParseNode> for_expr) {
    auto begin = for_expr->childs.begin();
    auto end = for_expr->childs.end();
    auto is_term = [](const std::unique_ptr<ParseNode> &ch) {
      return ch->tok.index() == 1; // is terminal
    };

    for_expr->childs.erase(std::remove_if(begin, end, is_term), end);
    erase_idx(for_expr->childs[0]->childs, 1); // remove ASSIGN_OP if it's there
    return for_expr;
  };

  auto get_name_from_cmd = [](std::unique_ptr<ParseNode> cmd) {
    if (cmd->tok.index() != 0 && std::get<NonTerm>(cmd->tok) != NonTerm::CMD) {
      throw std::runtime_error("bad usage of get_name_from_cmd");
    }

    if (cmd->childs.size() > 0) {
      auto out = std::move(cmd->childs[3]);
      cmd->childs.clear();
      return out;
    }

    auto out = std::make_unique<ParseNode>();
    out->tok = Token{TokenType::WORD, ""};
    return out;
  };

  tree->childs[0] = shrink_for(std::move(tree->childs[0]));
  if (std::get<NonTerm>(tree->childs[1]->tok) == NonTerm::STAT) {
    tree->childs[1] = shrink(std::move(tree->childs[1]));
  } else // this is cmd
  {
    erase_idx(tree->childs, 2); // remove semicolon
    tree->childs[1] = get_name_from_cmd(std::move(tree->childs[1]));
  }

  return tree;
}

void Interpreter::addSymbols(const std::unique_ptr<ParseNode> &tree) {
  // stat -> for -> init
  const auto &init = tree->childs[0]->childs[0];

  if (init->childs.size() > 0) {
    auto a = std::get<Token>(init->childs[0]->tok);
    auto b = std::get<Token>(init->childs[1]->tok);

    if (symbolTable[a.second].ind != -1) {
      throw SemanticError(a.second + " is initialized twice");
    }
    symbolTable[a.second].ind = _ind++;
    symbolTable[a.second].scope = _scope;
    symbolTable[a.second].init = b;
    _memory.push_back(0);
  }

  if (tree->childs[1]->tok.index() == 0) // i. e. == NonTerm::STAT
  {
    _scope++;
    addSymbols(tree->childs[1]);
  } else // == NonTerm::CMD
  {
    _word = std::get<Token>(tree->childs[1]->tok).second;
  }
}

void Interpreter::validate(const std::unique_ptr<ParseNode> &i) {
  const auto &init = i->childs[0]->childs[0];
  if (init->childs.size() > 0) {
    auto l = std::get<Token>(init->childs[0]->tok);
    auto r = std::get<Token>(init->childs[1]->tok);
    checks(l, cur_scope);
  }

  // comparing
  const auto &comp_part = i->childs[0]->childs[1];
  Token a, cmp, b;
  if (comp_part->childs.size() > 0) {
    a = std::get<Token>(comp_part->childs[0]->childs[0]->tok);
    cmp = std::get<Token>(comp_part->childs[1]->tok);
    b = std::get<Token>(comp_part->childs[2]->childs[0]->tok);
    checks(a, cur_scope);
    checks(b, cur_scope);
  }

  // action
  const auto &act = i->childs[0]->childs[2];
  if (act->childs.size() > 0) {
    auto name = std::get<Token>(act->childs[0]->tok);
    auto action = std::get<Token>(act->childs[1]->tok);

    if (name.first == TokenType::UNARY_OP)
      swap(name, action);
    checks(name, cur_scope);
  }
  if (i->childs[1]->tok.index() == 0 &&
      std::get<NonTerm>(i->childs[1]->tok) == NonTerm::STAT) // == STAT
  {
    cur_scope++;
    validate(i->childs[1]);
  }
}

void Interpreter::execute(const std::unique_ptr<ParseNode> &ast) {
  validate(ast);
  _execRoutine(ast);
}

void Interpreter::_execRoutine(const std::unique_ptr<ParseNode> &i) {
  const auto &init = i->childs[0]->childs[0];
  static int cur_scope = 0;
  if (init->childs.size() > 0) {
    auto l = std::get<Token>(init->childs[0]->tok);
    auto r = std::get<Token>(init->childs[1]->tok);
    if (r.first == TokenType::NUM) {
      _memory[symbolTable[l.second].ind] = std::stoi(r.second);
    } else {
      _memory[symbolTable[l.second].ind] = _memory[symbolTable[r.second].ind];
    }
  }

  // comparing
  const auto &comp_part = i->childs[0]->childs[1];
  Token a, cmp, b;
  int aa = 0, bb = 0;
  if (comp_part->childs.size() > 0) {
    a = std::get<Token>(comp_part->childs[0]->childs[0]->tok);
    cmp = std::get<Token>(comp_part->childs[1]->tok);
    b = std::get<Token>(comp_part->childs[2]->childs[0]->tok);
    aa = a.first == TokenType::NUM ? std::stoi(a.second)
                                   : _memory[symbolTable[a.second].ind];
    bb = b.first == TokenType::NUM ? std::stoi(b.second)
                                   : _memory[symbolTable[b.second].ind];
  }

  for (; comp[cmp.second](aa, bb);) {
    if (i->childs[1]->tok.index() == 0 &&
        std::get<NonTerm>(i->childs[1]->tok) == NonTerm::STAT) {
      _execRoutine(i->childs[1]);
    } else {
      if (_word != "")
        _stream.get() << _word << std::endl;
    }

    // action
    const auto &act = i->childs[0]->childs[2];
    if (act->childs.size() > 0) {
      auto name = std::get<Token>(act->childs[0]->tok);
      auto action = std::get<Token>(act->childs[1]->tok);

      if (name.first == TokenType::UNARY_OP)
        swap(name, action);

      if (action.second == "++") {
        _memory[symbolTable[name.second].ind]++;
      } else {
        _memory[symbolTable[name.second].ind]--;
      }
    }

    if (comp_part->childs.size() > 0) {
      aa = a.first == TokenType::NUM ? std::stoi(a.second)
                                     : _memory[symbolTable[a.second].ind];
      bb = b.first == TokenType::NUM ? std::stoi(b.second)
                                     : _memory[symbolTable[b.second].ind];
    }
  }
}
