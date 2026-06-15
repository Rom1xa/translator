/* Грамматика bison для языка вложенных for-циклов.
 *
 * BNF:
 *   <stat>       ::= <for-expr> <stat> | <for-expr> <cmd> ;
 *   <for-expr>   ::= for( <init> ; <cond> ; <action-str> )
 *   <init>       ::= NAME=NUM | NAME=NAME | e
 *   <cond>       ::= <operand> COMP <operand> | e
 *   <operand>    ::= NAME | NUM
 *   <action-str> ::= UNARY_OP NAME | NAME UNARY_OP | e
 *   <cmd>        ::= print(" WORD ") | e
 */

%{
#include <cstdio>
#include <string>

#include "ast.hpp"

extern int yylex();
extern int yylineno;
extern char *yytext;

void yyerror(const char *s);

ast::Program *g_root = nullptr;
int g_syntax_errors = 0;
%}

/* Семантические значения: указатели на узлы АСД и строки лексем.
 * std::string* и сырые указатели тривиально копируемы, что подходит
 * для union бизона. Узлы создаются через new и передаются вверх,
 * затем оборачиваются в unique_ptr родительского узла. */
%union {
  std::string    *str;
  ast::Program   *prog;
  ast::Stat      *stat;
  ast::ForExpr   *forexpr;
  ast::Init      *init;
  ast::Cond      *cond;
  ast::Operand   *operand;
  ast::ActionStr *action;
  ast::Cmd       *cmd;
}

%token <str> NAME NUM STRING COMP UNARY_OP
%token FOR PRINT LPAREN RPAREN SEMI EQ

%type <prog>    program
%type <stat>    stat
%type <forexpr> forExpr
%type <init>    init
%type <cond>    cond
%type <operand> operand
%type <action>  actionStr
%type <cmd>     cmd

%start program

%%

program
    : stat
        {
          g_root = new ast::Program();
          g_root->stat.reset($1);
          $$ = g_root;
        }
    ;

stat
    : forExpr stat
        {
          auto *n = new ast::NestedStat();
          n->forExpr.reset($1);
          n->body.reset($2);
          $$ = n;
        }
    | forExpr cmd SEMI
        {
          auto *n = new ast::LeafStat();
          n->forExpr.reset($1);
          n->cmd.reset($2);
          $$ = n;
        }
    ;

forExpr
    : FOR LPAREN init SEMI cond SEMI actionStr RPAREN
        {
          auto *f = new ast::ForExpr();
          f->init.reset($3);
          f->cond.reset($5);
          f->actionStr.reset($7);
          $$ = f;
        }
    ;

init
    : NAME EQ NUM
        {
          auto *i = new ast::Init();
          i->empty = false;
          i->lhs = *$1;
          i->rhs.reset(new ast::Operand(ast::Operand::NUM, *$3));
          $$ = i;
        }
    | NAME EQ NAME
        {
          auto *i = new ast::Init();
          i->empty = false;
          i->lhs = *$1;
          i->rhs.reset(new ast::Operand(ast::Operand::NAME, *$3));
          $$ = i;
        }
    |
        { $$ = new ast::Init(); }
    ;

cond
    : operand COMP operand
        {
          auto *c = new ast::Cond();
          c->empty = false;
          c->left.reset($1);
          c->op = *$2;
          c->right.reset($3);
          $$ = c;
        }
    |
        { $$ = new ast::Cond(); }
    ;

operand
    : NAME
        { $$ = new ast::Operand(ast::Operand::NAME, *$1); }
    | NUM
        { $$ = new ast::Operand(ast::Operand::NUM, *$1); }
    ;

actionStr
    : UNARY_OP NAME
        {
          auto *a = new ast::ActionStr();
          a->kind = ast::ActionStr::PREFIX;
          a->op = *$1;
          a->name = *$2;
          $$ = a;
        }
    | NAME UNARY_OP
        {
          auto *a = new ast::ActionStr();
          a->kind = ast::ActionStr::POSTFIX;
          a->name = *$1;
          a->op = *$2;
          $$ = a;
        }
    |
        { $$ = new ast::ActionStr(); }
    ;

cmd
    : PRINT LPAREN STRING RPAREN
        {
          auto *c = new ast::Cmd();
          c->empty = false;
          c->word = *$3;
          $$ = c;
        }
    |
        { $$ = new ast::Cmd(); }
    ;

%%

void yyerror(const char *s) {
  ++g_syntax_errors;
  std::fprintf(stderr, "Syntax error at line %d: %s\n", yylineno, s);
}
