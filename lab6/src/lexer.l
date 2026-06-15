/* Лексический анализатор flex для языка вложенных for-циклов. */

%option noyywrap yylineno nounput noinput

%{
#include <string>

#include "ast.hpp"
#include "parser.tab.hpp"

void yyerror(const char *s);
%}

%%

"for"                                       { return FOR; }
"print"                                     { return PRINT; }

"("                                         { return LPAREN; }
")"                                         { return RPAREN; }
";"                                         { return SEMI; }

"++"|"--"                                   { yylval.str = new std::string(yytext); return UNARY_OP; }

"<="|">="|"=="|"!="|"&&"|"||"|"<"|">"       { yylval.str = new std::string(yytext); return COMP; }

"="                                         { return EQ; }

[a-zA-Z_][a-zA-Z0-9_]*                      { yylval.str = new std::string(yytext); return NAME; }
[0-9]+                                      { yylval.str = new std::string(yytext); return NUM; }

 /* Строковый литерал "слово" — возвращаем содержимое без кавычек. */
\"[^\"\n]*\"                                { yylval.str = new std::string(yytext + 1, yyleng - 2); return STRING; }

 /* Пробельные символы игнорируются. */
[ \t\r\n]+                                  { }

.                                           { std::fprintf(stderr, "Lexical error at line %d: unexpected '%s'\n", yylineno, yytext); }

%%
