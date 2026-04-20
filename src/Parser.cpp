#include "Parser.hpp"
#include "Definitions.hpp"
#include "Errors.hpp"
#include <algorithm>

using row = std::array<Parser::State, getEnumSize<TokenType>()>;
using rrow = std::array<Parser::State, getEnumSize<NonTerm>()>;

std::array<std::array<Parser::State, getEnumSize<TokenType>()>, 33>
    Parser::_action = {
        row{12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        row{12, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, -15, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1},
        row{0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0},
        row{0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -14, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -2},
        row{0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, -6, 0},
        row{0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0},
        row{0, 0, 17, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -4, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -5, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 0},
        row{0, 0, 23, 0, 24, 0, 0, 0, 0, 0, 0, -8, 0},
        row{0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 23, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -7, 0},
        row{0, 0, 0, 0, 0, -9, 0, 0, 0, 0, 0, -9, 0},
        row{0, 0, 0, 0, 0, -10, 0, 0, 0, 0, 0, -10, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 0},
        row{0, 0, 29, 0, 0, 0, 27, 0, 0, 0, -13, 0, 0},
        row{0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -11, 0, 0},
        row{0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -12, 0, 0},
        row{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0},
        row{-3, 0, 0, 0, 0, 0, 0, 0, -3, 0, 0, -3, 0},
};

std::array<std::array<Parser::State, getEnumSize<NonTerm>()>, 27>
    Parser::_goto = {
        rrow{1, 2, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{3, 2, 0, 0, 0, 0, 10}, rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 18, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 25, 20, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 22, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 0, 0},  rrow{0, 0, 0, 0, 0, 0, 0},
        rrow{0, 0, 0, 0, 0, 31, 0},
};

std::array<std::pair<NonTerm, Parser::State>, 15> Parser::_products{
    std::pair{NonTerm::STAT, 2},       std::pair{NonTerm::STAT, 3},
    std::pair{NonTerm::FOR_EXPR, 8},   std::pair{NonTerm::INIT, 3},
    std::pair{NonTerm::INIT, 3},       std::pair{NonTerm::INIT, 0},
    std::pair{NonTerm::COND, 3},       std::pair{NonTerm::COND, 0},
    std::pair{NonTerm::OPERAND, 1},    std::pair{NonTerm::OPERAND, 1},
    std::pair{NonTerm::ACTION_STR, 2}, std::pair{NonTerm::ACTION_STR, 2},
    std::pair{NonTerm::ACTION_STR, 0}, std::pair{NonTerm::CMD, 6},
    std::pair{NonTerm::CMD, 0},
};

Parser::Parser(Scanner s) : _scanner(s) {}

std::unique_ptr<ParseNode> Parser::parse() {
  _stack.push({0, std::make_unique<ParseNode>()});
  while (true) {
    auto tok = _scanner.next();
    auto tok_idx = idx(tok.first);
    auto state_idx = _stack.top().first;

    if (state_idx == 1 && tok.first == TokenType::END) // Accept
    {
      // Возможно, такой хардкод принятия - не очень хорошо
      return std::move(_stack.top().second);
    }

    auto action = _action[state_idx][tok_idx];
    if (action == 0) // Error
    {
      throw SyntaxError("Error in syntax");
    }

    else if (action > 0) // Shift
    {
      auto to_push = std::make_unique<ParseNode>();
      to_push->tok = tok;
      _stack.push({action, std::move(to_push)});
    }

    else // action < 0 => Reduce
    {
      auto to_push = std::make_unique<ParseNode>();
      auto production = _products[std::abs(action) - 1];
      to_push->tok = production.first;
      for (int i = 0; i < production.second; ++i) {
        auto ch = std::move(_stack.top());
        _stack.pop();
        to_push->childs.push_back(std::move(ch.second));
      }

      // мы вытаскиваем элементы из стека в обратном порядке
      std::reverse(to_push->childs.begin(), to_push->childs.end());

      if (std::abs(action) == 4 || std::abs(action) == 5) // инициализация
      {
        symbolTable[std::get<Token>(to_push->childs[0]->tok).second] =
            Symbol{-1, {}, -1};
      }

      auto new_state = _goto[_stack.top().first][idx(production.first)];

      _stack.push({new_state, std::move(to_push)});

      _scanner.putback(tok);
    }
  }
}
