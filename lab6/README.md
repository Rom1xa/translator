# Лабораторная работа 6: анализатор Bison + Flex

**Вариант:** 13

Реализация на **Bison + Flex**: лексический и синтаксический
анализатор языка вложенных `for`-циклов с построением AST и интерпретацией.

## Описание языка

Язык описывает последовательность вложенных `for`-циклов в стиле C,
завершающихся командой `print("слово")`.

**Грамматика (BNF):**

```
<stat>       ::= <for-expr> <stat> | <for-expr> <cmd> ;
<for-expr>   ::= for( <init> ; <cond> ; <action-str> )
<init>       ::= NAME=NUM | NAME=NAME | ε
<cond>       ::= <operand> COMP <operand> | ε
<operand>    ::= NAME | NUM
<action-str> ::= UNARY_OP NAME | NAME UNARY_OP | ε
<cmd>        ::= print(" WORD ") | ε
```

**Операторы COMP:** `<`, `<=`, `>`, `>=`, `&&`, `||`, `==`, `!=`
**Унарные операторы:** `++`, `--`

## Структура проекта

```
src/
  lexer.l            — лексер Flex (токены)
  parser.y           — парсер Bison (грамматика + построение AST)
  ast.hpp / ast.cpp  — узлы AST и их печать
  interpreter.hpp/cpp— исполнение AST
  main.cpp           — точка входа (ввод из файла)
tests/               — 8 тестов (4 positive + 4 negative)
run_tests.sh         — прогон тестов с проверкой кодов возврата
Makefile             — сборка и тесты
Dockerfile           — воспроизводимая сборка
```

## Сборка и запуск (Linux)

Нужны `bison`, `flex`, `g++` (C++17), `make`.

```bash
make all                       # собрать build/forlang
./build/forlang tests/positive_basic.txt
make test                      # прогнать все тесты
make clean
```

Формат запуска: `forlang <input-file>`. При запуске без аргумента выводится
краткая справка. Программа печатает построенное AST (секция `=== AST ===`) и
результат исполнения (секция `=== OUTPUT ===`).

Коды возврата:

- `0` — успех;
- `1` — ошибка аргументов / не удалось открыть файл;
- `2` — синтаксическая или семантическая ошибка.

## Запуск через Docker

```bash
docker build -t forlang-lab6 .
docker run --rm forlang-lab6 make test                       # все тесты
docker run --rm -v "$(pwd)/tests:/in" forlang-lab6 ./build/forlang /in/positive_basic.txt
```

## Примеры входных программ

```
for(i=0;i<10;i++) for(j=0;j<i;j++) print("bob");
for(i=0;i<3;++i) print("hello");
for(;;) print("inf");   # семантическая ошибка: бесконечный цикл (лимит итераций)
for(;n<5;n++) print("x"); # семантическая ошибка: переменная n не объявлена
```

## Тесты

| Файл                           | Ожидание              |
| ------------------------------ | --------------------- |
| positive_basic.txt             | "hello" × 3           |
| positive_nested.txt            | "ab" × 6              |
| positive_prefix_unary.txt      | "x" × 5               |
| positive_name_name_init.txt    | "hi" × 6              |
| negative_missing_semicolon.txt | синтаксическая ошибка |
| negative_missing_paren.txt     | синтаксическая ошибка |
| negative_bare_print.txt        | синтаксическая ошибка |
| negative_undefined_var.txt     | семантическая ошибка  |

## Особенности реализации

- **Лексер (Flex):** ключевые слова `for`/`print`, пунктуация, операторы
  сравнения и унарные, идентификаторы, числа и строковый литерал `"..."`
  (содержимое возвращается без кавычек). Неизвестные символы — лексическая ошибка.
- **Парсер (Bison):** LALR(1)-грамматика без конфликтов; AST строится прямо в
  семантических действиях. Синтаксические ошибки сообщаются с номером строки.
- **Интерпретатор:** обходит AST, хранит переменные в `map<string,int>`,
  поддерживает вложенные циклы. Проверяет необъявленные переменные и
  ограничивает защита от бесконечного цикла).
