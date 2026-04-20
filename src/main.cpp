#include "Definitions.hpp"
#include "Interpreter.hpp"
#include "Parser.hpp"
#include <fstream>
#include <iostream>

std::unordered_map<TokenType, std::string> desc{
    {TokenType::ASSIGN_OP, "ASSIGN_OP"},
    {TokenType::COMP, "COMP"},
    {TokenType::END, "END"},
    {TokenType::For, "For"},
    {TokenType::LPAR, "LPAR"},
    {TokenType::NAME, "NAME"},
    {TokenType::NUM, "NUM"},
    {TokenType::Print, "Print"},
    {TokenType::QUOTE, "QUOTE"},
    {TokenType::RPAR, "RPAR"},
    {TokenType::SEMICOL, "SEMICOL"},
    {TokenType::UNARY_OP, "UNARY_OP"},
    {TokenType::WORD, "WORD"}};

std::unordered_map<NonTerm, std::string> nt_desc{
    {NonTerm::ACTION_STR, "action-str"},
    {NonTerm::CMD, "cmd"},
    {NonTerm::COND, "cond"},
    {NonTerm::FOR_EXPR, "for"},
    {NonTerm::INIT, "init"},
    {NonTerm::OPERAND, "operand"},
    {NonTerm::STAT, "stat"},
};

void print_tree(const std::unique_ptr<ParseNode> &p, int depth = 0) {
  auto align = std::string(depth, ' ');
  if (p->tok.index() == 0) // NonTerm
  {
    std::cout << align << nt_desc[std::get<0>(p->tok)] << std::endl;
    for (const auto &i : p->childs) {
      print_tree(i, depth + 4);
    }
  } else // Term
  {
    const auto &token = std::get<1>(p->tok);
    std::cout << align << desc[token.first] << " " << token.second << std::endl;
  }
}

int main(int argc, char **argv) {
  if (argc == 2) {
    if (std::string(argv[1]) == "--help") {
      std::cout << "tranlator - простой транслятор for-выражений" << std::endl;
      std::cout << "При вызове без аргументов читает стандартный вывод, "
                   "пока не будет нажато Ctrl+D"
                << std::endl;
      std::cout << "При вызове с аргументом, транслятор читает код из файла"
                << std::endl;
      return 0;
    }
  }

  // Можно сделать конструктор Scanner и для rvalue
  std::fstream s;
  Scanner scanner;
  if (argc == 2) {
    s.open(argv[1], std::ios_base::in);
    if (!s.is_open()) {
      std::cerr << "Ошибка при чтении файла " + std::string(argv[1])
                << std::endl;
      return 1;
    }
    scanner = Scanner(s);
  }
  try {
    Parser parser(scanner);
    Interpreter interpreter;
    auto i = parser.parse();
    i = interpreter.shrink(std::move(i));
    interpreter.addSymbols(i);
    interpreter.execute(i);
  } catch (std::exception &e) {
    std::cerr << "Ошибка при анализе программы: " << std::endl;
    std::cerr << "Сообщение ошибки: " << e.what() << std::endl;
    return 1;
  }
}
