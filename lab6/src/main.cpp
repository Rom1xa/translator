#include <cstdio>
#include <iostream>

#include "ast.hpp"
#include "interpreter.hpp"

extern int yyparse();
extern FILE *yyin;
extern ast::Program *g_root;
extern int g_syntax_errors;

int main(int argc, char **argv) {
  if (argc < 2) {
    std::fprintf(stderr, "Usage: forlang <input-file>\n");
    return 1;
  }

  const char *path = argv[1];
  FILE *input = std::fopen(path, "r");
  if (!input) {
    std::fprintf(stderr, "Cannot open file: %s\n", path);
    return 1;
  }

  yyin = input;
  int parseResult = yyparse();
  std::fclose(input);

  if (parseResult != 0 || g_syntax_errors > 0 || g_root == nullptr) {
    std::fprintf(stderr, "Parsing failed with %d error(s)\n", g_syntax_errors);
    return 2;
  }

  std::cout << "=== AST ===\n";
  g_root->print(std::cout, 0);

  std::cout << "\n=== OUTPUT ===\n" << std::flush;
  Interpreter interpreter;
  return interpreter.execute(*g_root);
}
