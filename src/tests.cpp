#include "Definitions.hpp"
#include "Errors.hpp"
#include "Interpreter.hpp"
#include "Parser.hpp"
#include "Scanner.hpp"
#include <exception>
#include <sstream>
#include <vector>

using test_suite = std::function<std::pair<bool, std::string>()>;

// fixture
std::stringstream in;
std::stringstream out;
Scanner scanner(in);
Parser parser(scanner);
Interpreter interpreter(out);

std::string make_err(std::string cs, int exp, int found) {
  return cs + ", not ok: expected - " + std::to_string(exp) + ", found - " +
         std::to_string(found);
}

void exec() {
  auto tree = parser.parse();
  tree = interpreter.shrink(std::move(tree));
  interpreter.addSymbols(tree);
  interpreter.execute(tree);
}

// also it flushes out
int cnt_lines() {
  int lines = 0;
  std::string stub;
  while (out >> stub) {
    lines++;
  }
  return lines;
}

test_suite basic_case = []() {
  std::string name = "basic_case";
  in << "for(i=0;i<10;++i) print(\"basic\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 10;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 10, lines)};
};

test_suite two_cycles = []() {
  std::string name = "two_cycles";
  in << "for(i=0;i<10;++i) for(j=0;j<10;++j) print(\"two\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 100;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 100, lines)};
};

test_suite three_cycles = []() {
  std::string name = "three_cycles";
  in << "for(i=0;i<3;++i) for(j=0;j<3;++j) for(k=0;k<3;++k) "
        "print(\"three_cycles\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 27;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 27, lines)};
};

test_suite four_cycles = []() {
  std::string name = "four_cycles";
  in << "for(i=0;i<2;++i) for(j=0;j<2;++j) for(k=0;k<2;++k) "
        "for(a=0;a<2;++a) print(\"four\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 16;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 16, lines)};
};

test_suite decrement = []() {
  std::string name = "decrement";
  in << "for (i=10;i>0;i--) print(\"dec\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 10;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 10, lines)};
};

test_suite logical_ops = []() {
  std::string name = "logical_ops";
  in << "for (a=1; a || 0; a--) for (b = 1; b && 1; b--) print(\"ops\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 1;
  return std::pair{verdict, verdict ? name + ": ok" : make_err(name, 1, lines)};
};

test_suite test_greater_or_eq = []() {
  std::string name = "test_greater_or_eq";
  in << "for (i=0;i<=10;++i) print(\"goreq\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 11;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 11, lines)};
};

test_suite test_lower_or_eq = []() {
  std::string name = "test_lower_or_eq";
  // Отмечу, что если бы был предел i >= 0, то это бы всегда было верно, так
  // как числа в языке беззнаковые
  in << "for (i=11;i>=1;--i) print(\"lower_or_eq\");";

  try {
    exec();
  } catch (std::exception &e) {
    std::pair{false, name + ": throw unexpected exception, errmgs: " +
                         std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 11;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 11, lines)};
};

test_suite test_nei = []() {
  std::string name = "test_nei";
  // Отмечу, что если бы был предел i >= 0, то это бы всегда было верно, так
  // как числа в языке беззнаковые
  in << "for (i=0;i<10;++i) for(j=0;j!=i;++j) print(\"noti\");";

  try {
    exec();
  } catch (std::exception &e) {
    return std::pair{false, name + ": throw unexpected exception, errmgs: " +
                                std::string(e.what())};
  }
  auto lines = cnt_lines();

  bool verdict = lines == 45;
  return std::pair{verdict,
                   verdict ? name + ": ok" : make_err(name, 45, lines)};
};

test_suite wrong_lexem = []() {
  std::string name = "wrong_lexem";
  in << "for (\\123);";

  try {
    exec();
  } catch (LexicalError &e) {
    return std::pair{true, name + ": ok"};
  }
  return std::pair{false, name + ": expected LexicalError"};
};

test_suite syntax_error = []() {
  std::string name = "syntax_error";
  in << "for (;;++ ++);";

  try {
    exec();
  } catch (SyntaxError &e) {
    return std::pair{true, name + ": ok"};
  } catch (std::exception &e) {
    return std::pair{false, name + ": throw unexpected exception, errmgs: " +
                                std::string(e.what())};
  }

  return std::pair{false, name + ": expected SyntaxError"};
};

test_suite isnt_initialized = []() {
  std::string name = "isnt_initialized";
  in << "for(;i<2;++i) print(\"abra\");";

  try {
    exec();
  } catch (SemanticError &e) {
    return std::pair{true, name + ": ok, errmsg: " + e.what()};
  } catch (std::exception &e) {
    return std::pair{false, name + ": throw unexpected exception, errmgs: " +
                                std::string(e.what())};
  }
  return std::pair{false, name + ": expected SemanticError"};
};

test_suite out_of_scope = []() {
  std::string name = "out_of_scope";
  in << "for(i=0;j<2;++i) for(j=0;j<3;++j) print(\"outofscope\");";

  try {
    exec();
  } catch (SemanticError &e) {
    return std::pair{true, name + ": ok, errmsg: " + e.what()};
  } catch (std::exception &e) {
    return std::pair{false, name + ": throw unexpected exception, errmgs: " +
                                std::string(e.what())};
  }
  return std::pair{false, name + ": expected SemanticError"};
};

test_suite init_twice = []() {
  std::string name = "init_twice";
  in << "for(i=0;i<2;++i) for(i=0;i<3;++i) print(\"init twice\");";

  try {
    exec();
  } catch (SemanticError &e) {
    return std::pair{true, name + ": ok, errmsg: " + e.what()};
  } catch (std::exception &e) {
    return std::pair{false, name + ": throw unexpected exception, errmgs: " +
                                std::string(e.what())};
  }
  return std::pair{false, name + ": expected SemanticError"};
};

int main() {
  std::vector<test_suite> suites = {
      basic_case,   two_cycles,  three_cycles,       four_cycles,
      decrement,    logical_ops, test_greater_or_eq, test_lower_or_eq,
      test_nei,     wrong_lexem, syntax_error,       isnt_initialized,
      out_of_scope, init_twice};

  int cnt = 1, ok = 0;
  for (auto &suite : suites) {
    // reset
    in.str("");
    in.clear();
    out.str("");
    out.clear();
    scanner = Scanner(in);
    parser = Parser(scanner);
    interpreter = Interpreter(out);

    auto done = suite();
    if (done.first)
      ok++;
    std::cout << cnt++ << "# " << done.second << std::endl;
    symbolTable.clear(); // after execution becaus it's common
  }
  std::cout << "done: " << cnt - 1 << " tests, ok: " << ok << " tests"
            << std::endl;
}
