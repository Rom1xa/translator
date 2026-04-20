# Лабораторные работы курса "Основы проектирование трансляторов"

**Абакумов Роман**
**КМБО-05-23**
Вариант: **13**

## Задание 1

Даны выражения из операторов for (пример: `for(i=0;i<10;i++) for(j=0;j<i;j++) print("bob");`). Разработать для данных выражений грамматику в BNF.

Выражения представляют собой последовательности циклов for, подобных тому, какие есть в C или C++. Если за циклом идет второй цикл, то второй цикл _вложен_ в первый. После циклов идет инструкция вывода строки print("word"), где word - произвольное слово, состоящее из символов.

Цикл начинается ключевым словом for, и в круглых скобках идет три части, разделенные точкой с запятой. Первая секция - _секция инициализации_. В ней можно объявить целочисленную переменную и присвоить ей некоторое значение. Можно оставить секцию пустой. Также можно оказать несколько целочисленных переменных через запятую.

Вторая секция - _секция условия_. Пока условие в секции истинно, цикл выполняется. При формировании условия можно использовать следующие символы: <, <=, >, >=, &&, ||, ==, != - их смысл такой же, как в Си. Все операции правоассоциативные. У операций есть приоритет, такой же, как в Си.

Третья секция - _секция действия_. В конце цикла выполняется это условие. Условием может быть операции постфиксного\префиксного инкремента\декремента. В этом цикле можно присваивать значения одной переменной, или нескольких, через запятую.

Примеры:

- `for(;;) print("bob");` - печатает "bob" бесконечно
- `for(i=0;i<10;++i) print("beb");` - печатает "beb" 10 раз
- `for(i=0;i<10;++i) for(j=0;i<5 && j<5;++j) print("bib");` - печатает "bib" 25 раз
- `for(i=1;i && 1;i--) print("goal");` - печатает "goal" 1 раз

Далее, _курсивом_ выделены нетерминалы.

Токены:

- NAME*TOKEN - последовательность строчных и прописных латинских букв, цифр и символа '*', не начинающаяся с цифры
- WORD_TOKEN - последовательность символов, кроме кавычек
- NUM_TOKEN - десятичное число
- COMP_TOKEN - < | <= | > | >= | && | || | == | !=
- UNARY_OP_TOKEN - ++ | --

_\<stat>_ ::= _\<for-expr>_ _\<stat>_ | _\<for-expr>_ _\<cmd>_;
_\<for-expr>_ ::= for(_\<init>_;_\<cond>_;_\<action-str>_)

_\<init>_ ::= NAME_TOKEN=NUM_TOKEN | NAME_TOKEN=NAME_TOKEN | ε

_\<cond>_ ::= _\<operand>_ COMP*TOKEN*\<operand>_| ε
_\<operand>\_ ::= NAME_TOKEN | NUM_TOKEN

_\<action-str>_ ::= UNARY_OP_TOKEN NAME_TOKEN | NAME_TOKEN UNARY_OP_TOKEN | ε

_\<cmd>_ ::= print(" WORD_TOKEN ") | ε

## Задание 2

Определить класс и однозначность языка и грамматики

Начнем с класса грамматики. Правило регулярного языка предполагает, что оно будет иметь вид _F ::= Aβ_, где A - нетерминал, а β - произвольная строка терминалов. Так что, уже первое правило не подходит под определение, так как в продукции один нетерминал переходит в два. Однако, грамматика контекстно-свободная, так как у всех правил слева стоят нетерминалы.

Теперь про класс языка. Составим следующее регулярное выражение:

```
(for\(((NAME_TOKEN)=(OPERAND))?;((OPERAND)(ACTION)(OPERAND))?;((\+\+|--)(NAME_TOKEN)|(NAME_TOKEN)(\+\+|--))?\) )+(print\("(WORD)"\))*;

NAME_TOKEN=[a-zA-Z_]\w*
OPERAND=[a-zA-Z_]\w*|[1-9]\d*|0
ACTION=<|<=|>|>=|&&|\|\||==|!=
WORD=[^"]*
```

[Ссылка на regex101](https://regex101.com/r/ny7z15/2)

Язык можно описать регулярным выражением - значит, что язык регулярный.

Грамматика является однозначной, так для нее можно построить LR(1)-анализатор, что будет сделано в задании 4. Поскольку язык порождается однозначной грамматикой, язык также является однозначным.

## Задание 3

Построить синтаксическое дерево и вывод произвольного предложения

Допустим, у нас имеется предложение `for(i=0;i<10;i++) for(j=0;j<i;j++) print("bob");`. Покажем вывод этого выражения. Далее под <...> подразумевается разобранный внешний цикл:

**stat**  
**for-expr** **stat**
for(**init**;**cond**;**action-str**) **stat**  
for(NAME_TOKEN=NUM_TOKEN;**cond**;**action-str**) **stat**  
for(NAME_TOKEN=NUM_TOKEN;**operand** COMP_TOKEN **operand**;**action-str**) **stat**  
for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN **operand**;**action-str**) **stat**  
for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN NUM_TOKEN;**action-str**) **stat**  
for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN NUM_TOKEN;NAME_TOKEN UNARY_OP_TOKEN) **stat**  
<...> for(**init**;**cond**;**action-str**) **cmd**  
<...> for(NAME_TOKEN=NUM_TOKEN;**cond**;**action-str**) **cmd**  
<...> for(NAME_TOKEN=NUM_TOKEN;**operand** COMP_TOKEN **operand**;**action-str**) **cmd**  
<...> for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN **operand**;**action-str**) **cmd**  
<...> for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN NAME_TOKEN;**action-str**) **cmd**  
<...> for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN NAME_TOKEN;NAME_TOKEN UNARY_OP_TOKEN) **cmd**  
<...> for(NAME_TOKEN=NUM_TOKEN;NAME_TOKEN COMP_TOKEN NAME_TOKEN;NAME_TOKEN UNARY_OP_TOKEN) print(" WORD_TOKEN ");

Синтаксическое дерево:

```
<stat> ┐
       ├ <for-expr> ┐
       │            ├ "for"
       │            ├ "("
       │            ├ <init> ┐
       │            │        ├ NAME_TOKEN
       │            │        ├ "="
       │            │        └ NUM_TOKEN
       │            │
       │            ├ ";"
       │            ├ <cond> ┐
       │            │        ├ <operand> - NAME_TOKEN
       │            │        ├ COMP_TOKEN
       │            │        └ <operand> - NUM_TOKEN
       │            ├ ";"
       │            ├ <action-str> ┐
       │            │              ├ NAME_TOKEN
       │            │              └ UNARY_OP_TOKEN
       │            └ ")"
       │
       └ <stat> ┐
                ├ <for-expr> ┐
                │            ├ "for"
                │            ├ "("
                │            ├ <init> ┐
                │            │        ├ NAME_TOKEN
                │            │        ├ "="
                │            │        └ NUM_TOKEN
                │            │
                │            ├ ";"
                │            ├ <cond> ┐
                │            │        ├ <operand> - NAME_TOKEN
                │            │        ├ COMP_TOKEN
                │            │        └ <operand> - NAME_TOKEN
                │            ├ ";"
                │            ├ <action-str> ┐
                │            │              ├ NAME_TOKEN
                │            │              └ UNARY_OP_TOKEN
                │            └ ")"
                │
                └ <cmd> ┐
                        ├ "print("
                        ├ WORD_TOKEN
                        ├ ")"
                        └ ";"
```

## Задание 4

Построить SLR-таблицу для данной грамматики

### Нахождение состояний LR(0)-автомата

Для начала расширим грамматику, введя начальный символ:

```
<stat'> ::= <stat>

Найдем I_0 = Closure({ <stat'> ::= .<stat> }) =
    {
        <stat'>    ::= .<stat>,
        <stat>     ::= .<for-expr> <stat>,
        <stat>     ::= .<for-expr> <cmd>;
        <for-expr> ::= .for(<init>;<cond>;<action-str>)
    }

Теперь ищем остальные состояния:

I_1 = Goto(I_0, <stat>) =
    Closure({ <stat'> ::= <stat>. }) =
    { <stat'> ::= <stat>. }

I_2 = Goto(I_0, <for-expr>) =
    Closure({
        <stat> ::= <for-expr> .<stat>,
        <stat> ::= <for-expr> .<cmd>
    }) =
    {
        <stat> ::= <for-expr> .<stat>,
        <stat> ::= <for-expr> .<cmd>;
        <cmd>  ::= .print(" WORD_TOKEN "),
        <cmd>  ::= .
        <stat> ::= .<for-expr> <stat>,
        <stat> ::= .<for-expr> <cmd>;
        <for-expr> ::= .for(<init>;<cond>;<action-str>)
    }

I_3 = Goto(I_2, <stat>) =
    { <stat> ::= <for-expr> <stat>. }

I_4 = Goto(I_2, print) =
    { <cmd> ::= print.(" WORD_TOKEN ") }

I_5 = Goto(I_4, "(") =
    { <cmd> ::= print(. " WORD_TOKEN ") }

I_6 = Goto(I_5, "\"") =
    { <cmd> ::= print(" . WORD_TOKEN ") }

I_7 = Goto(I_6, WORD_TOKEN) =
    { <cmd> ::= print(" WORD_TOKEN .") }

I_8 = Goto(I_7, "\"") =
    { <cmd> ::= print(" WORD_TOKEN ".) }

I_9 = Goto(I_8, ")") =
    { <cmd> ::= print(" WORD_TOKEN "). }

I_10 = Goto(I_2, <cmd>) =
    { <stat> ::= <for-expr> <cmd>. ; }

I_11 = Goto(I_10, ";") =
    { <stat> ::= <for-expr> <cmd>;. }

Goto(I_2, <for-expr>) = Goto(I_0, <for-expr>) = I_2

I_12 = Goto(I_0, for) =
    { <for-expr> ::= for.(<init>;<cond>;<action-str>) }

Goto(I_2, for) = Goto(I_0, for) = I_12

I_13 = Goto(I_12, "(") =
    Closure({
        <for-expr> ::= for(.<init>;<cond>;<action-str>)
    }) =
    {
        <for-expr> ::= for(.<init>;<cond>;<action-str>),
        <init>     ::= .NAME_TOKEN=NUM_TOKEN,
        <init>     ::= .NAME_TOKEN=NAME_TOKEN
        <init>     ::= .
    }

I_14 = Goto(I_13, NAME_TOKEN) =
    {
        <init> ::= NAME_TOKEN.=NUM_TOKEN,
        <init> ::= NAME_TOKEN.=NAME_TOKEN
    }

I_15 = Goto(I_14, "=") =
    {
        <init> ::= NAME_TOKEN=.NUM_TOKEN,
        <init> ::= NAME_TOKEN=.NAME_TOKEN
    }

I_16 = Goto(I_15, NUM_TOKEN) =
    { <init> ::= NAME_TOKEN=NUM_TOKEN. }

I_17 = Goto(I_16, NAME_TOKEN) =
    { <init> ::= NAME_TOKEN=NAME_TOKEN. }

I_18 = Goto(I_7, <init>) =
    { <for-expr> ::= for(<init>.;<cond>;<action-str>) }

I_19 = Goto(I_18, ;) =
    Closure({
        <for-expr> ::= for(<init>;.<cond>;<action-str>)
    }) =
    {
        <for-expr> ::= for(<init>;.<cond>;<action-str>),
        <cond>     ::= .<operand> COMP_TOKEN <operand>,
        <operand>  ::= .NAME_TOKEN,
        <operand>  ::= .NUM_TOKEN,
        <cond>     ::= .
    }

I_20 = Goto(I_19, <operand>) =
    { <cond> ::= <operand> .COMP_TOKEN <operand> }

I_21 = Goto(I_20, COMP_TOKEN) =
    {
        <cond>    ::= <operand> COMP_TOKEN .<operand>,
        <operand> ::= .NAME_TOKEN,
        <operand> ::= .NUM_TOKEN,
    }

I_22 = Goto(I_21, <operand>) =
    { <cond> ::= <operand> COMP_TOKEN <operand>. }

I_23 = Goto(I_19, NAME_TOKEN) =
    { <operand> ::= NAME_TOKEN. }

I_24 = Goto(I_19, NUM_TOKEN) =
    { <operand> ::= NUM_TOKEN. }

Goto(I_21, NAME_TOKEN) = Goto(I_19, NAME_TOKEN) = I_23
Goto(I_21, NUM_TOKEN)  = Goto(I_19, NUM_TOKEN)  = I_24

I_25 = Goto(I_19, <cond>) =
    { <for-expr> ::= for(<init>;<cond>.;<action-str>) }

I_26 = Goto(I_25, ;) =
    Closure({
        <for-expr> ::= for(<init>;<cond>;.<action-str>)
    }) =
    {
        <for-expr>    ::= for(<init>;<cond>;.<action-str>),
        <action-str>  ::= .UNARY_OP_TOKEN NAME_TOKEN,
        <action-str>  ::= .NAME_TOKEN UNARY_OP_TOKEN
        <action-str>  ::= .
    }

I_27 = Goto(I_26, UNARY_OP_TOKEN) =
    { <action-str> ::= UNARY_OP_TOKEN .NAME_TOKEN }

I_28 = Goto(I_27, NAME_TOKEN) =
    { <action-str> ::= UNARY_OP_TOKEN NAME_TOKEN. }

I_29 = Goto(I_26, NAME_TOKEN) =
    { <action-str> ::= NAME_TOKEN .UNARY_OP_TOKEN }

I_30 = Goto(I_29, UNARY_OP_TOKEN) =
    { <action-str> ::= NAME_TOKEN UNARY_OP_TOKEN. }

I_31 = Goto(I_26, <action-str>) =
    { <for-expr> ::= for(<init>;<cond>;<action-str>.) }

I_32 = Goto(I_31, )) =
    { <for-expr> ::= for(<init>;<cond>;<action-str>). }
```

### Построение SLR-таблицы

Для начала отметим следующее:
_FOLLOW(<stat>) = { $ }
_ FOLLOW(<cmd>) = FOLLOW(<init>) = FOLLOW(<cond>) = { ; }
_FOLLOW(<action-str>) = { ) }
_ FOLLOW(<operand>) = { COMP_TOKEN, ; } \* FOLLOW(<for-expr>) = {print, for, ;}

Теперь, наконец, составим SLR-таблицу:
ACTION

|     | for | (   | NAME | WORD | NUM | COMP | UNARY_OP | =   | print | "   | )   | ;   | $   |
| --- | --- | --- | ---- | ---- | --- | ---- | -------- | --- | ----- | --- | --- | --- | --- |
| 0   | s12 |     |      |      |     |      |          |     |       |     |     |     |     |
| 1   |     |     |      |      |     |      |          |     |       |     |     |     | acc |
| 2   | s12 |     |      |      |     |      |          |     | s4    |     |     | r15 |     |
| 3   |     |     |      |      |     |      |          |     |       |     |     |     | r1  |
| 4   |     | s5  |      |      |     |      |          |     |       |     |     |     |     |
| 5   |     |     |      |      |     |      |          |     |       | s6  |     |     |     |
| 6   |     |     |      | s7   |     |      |          |     |       |     |     |     |     |
| 7   |     |     |      |      |     |      |          |     |       | s8  |     |     |     |
| 8   |     |     |      |      |     |      |          |     |       |     | s9  |     |     |
| 9   |     |     |      |      |     |      |          |     |       |     |     | r14 |     |
| 10  |     |     |      |      |     |      |          |     |       |     |     | s11 |     |
| 11  |     |     |      |      |     |      |          |     |       |     |     |     | r2  |
| 12  |     | s13 |      |      |     |      |          |     |       |     |     |     |     |
| 13  |     |     | s14  |      |     |      |          |     |       |     |     | r6  |     |
| 14  |     |     |      |      |     |      |          | s15 |       |     |     |     |     |
| 15  |     |     | s17  |      | s16 |      |          |     |       |     |     |     |     |
| 16  |     |     |      |      |     |      |          |     |       |     |     | r4  |     |
| 17  |     |     |      |      |     |      |          |     |       |     |     | r5  |     |
| 18  |     |     |      |      |     |      |          |     |       |     |     | s19 |     |
| 19  |     |     | s23  |      | s24 |      |          |     |       |     |     | r8  |     |
| 20  |     |     |      |      |     | s21  |          |     |       |     |     |     |     |
| 21  |     |     | s23  |      | s24 |      |          |     |       |     |     |     |     |
| 22  |     |     |      |      |     |      |          |     |       |     |     | r7  |     |
| 23  |     |     |      |      |     | r9   |          |     |       |     |     | r9  |     |
| 24  |     |     |      |      |     | r10  |          |     |       |     |     | r10 |     |
| 25  |     |     |      |      |     |      |          |     |       |     |     | s26 |     |
| 26  |     |     | s29  |      |     |      | s27      |     |       |     | r13 |     |     |
| 27  |     |     | s28  |      |     |      |          |     |       |     |     |     |     |
| 28  |     |     |      |      |     |      |          |     |       |     | r11 |     |     |
| 29  |     |     |      |      |     |      | s30      |     |       |     |     |     |     |
| 30  |     |     |      |      |     |      |          |     |       |     | r12 |     |     |
| 31  |     |     |      |      |     |      |          |     |       |     | s32 |     |     |
| 32  | r3  |     |      |      |     |      |          |     | r3    |     |     | r3  |     |

Таблица GOTO

|     | stat | for | init | cond | operand | action-str | cmd |
| --- | ---- | --- | ---- | ---- | ------- | ---------- | --- |
| 0   | 1    | 2   |      |      |         |            |     |
| 2   | 3    | 2   |      |      |         |            | 10  |
| 13  |      |     | 18   |      |         |            |     |
| 19  |      |     |      | 25   | 20      |            |     |
| 21  |      |     |      |      | 22      |            |     |
| 26  |      |     |      |      |         | 31         |     |

Для сверток используется следующая нумерация продукций:

1. _\<stat>_ ::= _\<for-expr>_ _\<stat>_
2. _\<stat>_ ::= _\<for-expr>_ _\<cmd>_;
3. _\<for-expr>_ ::= for(_\<init>_;_\<cond>_;_\<action-str>_)
4. _\<init>_ ::= NAME_TOKEN=NUM_TOKEN
5. _\<init>_ ::= NAME_TOKEN=NAME_TOKEN
6. _\<init>_ ::= ε
7. _\<cond>_ ::= _\<operand>_ COMP*TOKEN*\<operand>\_
8. _\<cond>_ ::= ε
9. _\<operand>_ ::= NAME_TOKEN
10. _\<operand>_ ::= NUM_TOKEN
11. _\<action-str>_ ::= UNARY_OP_TOKEN NAME_TOKEN
12. _\<action-str>_ ::= NAME_TOKEN UNARY_OP_TOKEN
13. _\<action-str>_ ::= ε
14. _\<cmd>_ ::= print(" WORD_TOKEN ")
15. _\<cmd>_ ::= ε

### Дополнение: LR-разбор строки из примера

Пример LR-анализа строки из примера:

| Строка разбора                                   | Стек состояний              | Действие    |
| ------------------------------------------------ | --------------------------- | ----------- |
| for(i=0;i<10;i++) for(j=0;j<i;j++) print("bob"); | 0                           | Shift(I_12) |
| (i=0;i<10;i++) for(j=0;j<i;j++) print("bob");    | 0 12                        | Shift(I_13) |
| i=0;i<10;i++) for(j=0;j<i;j++) print("bob");     | 0 12 13                     | Shift(I_14) |
| =0;i<10;i++) for(j=0;j<i;j++) print("bob");      | 0 12 13 14                  | Shift(I_15) |
| 0;i<10;i++) for(j=0;j<i;j++) print("bob");       | 0 12 13 14 15               | Shift(I_16) |
| ;i<10;i++) for(j=0;j<i;j++) print("bob");        | 0 12 13 14 15 16            | Reduce(4)   |
| ;i<10;i++) for(j=0;j<i;j++) print("bob");        | 0 12 13 18                  | Shift(I_19) |
| i<10;i++) for(j=0;j<i;j++) print("bob");         | 0 12 13 18 19               | Shift(I_23) |
| <10;i++) for(j=0;j<i;j++) print("bob");          | 0 12 13 18 19 23            | Reduce(9)   |
| <10;i++) for(j=0;j<i;j++) print("bob");          | 0 12 13 18 19 20            | Shift(I_21) |
| 10;i++) for(j=0;j<i;j++) print("bob");           | 0 12 13 18 19 20 21         | Shift(I_24) |
| ;i++) for(j=0;j<i;j++) print("bob");             | 0 12 13 18 19 20 21 24      | Reduce(10)  |
| ;i++) for(j=0;j<i;j++) print("bob");             | 0 12 13 18 19 20 21 22      | Reduce(7)   |
| ;i++) for(j=0;j<i;j++) print("bob");             | 0 12 13 18 19 25            | Shift(I_26) |
| i++) for(j=0;j<i;j++) print("bob");              | 0 12 13 18 19 25 26         | Shift(I_29) |
| ++) for(j=0;j<i;j++) print("bob");               | 0 12 13 18 19 25 26 29      | Shift(I_30) |
| ) for(j=0;j<i;j++) print("bob");                 | 0 12 13 18 19 25 26 29 30   | Reduce(12)  |
| ) for(j=0;j<i;j++) print("bob");                 | 0 12 13 18 19 25 26 31      | Shift(I_32) |
| for(j=0;j<i;j++) print("bob");                   | 0 12 13 18 19 25 26 31 32   | Reduce(3)   |
| for(j=0;j<i;j++) print("bob");                   | 0 2                         | Shift(I_12) |
| (j=0;j<i;j++) print("bob");                      | 0 2 12                      | Shift(I_13) |
| j=0;j<i;j++) print("bob");                       | 0 2 12 13                   | Shift(I_14) |
| =0;j<i;j++) print("bob");                        | 0 2 12 13 14                | Shift(I_15) |
| 0;j<i;j++) print("bob");                         | 0 2 12 13 14 15             | Shift(I_16) |
| ;j<i;j++) print("bob");                          | 0 2 12 13 14 15 16          | Reduce(4)   |
| ;j<i;j++) print("bob");                          | 0 2 12 13 18                | Shift(I_19) |
| j<i;j++) print("bob");                           | 0 2 12 13 18 19             | Shift(I_23) |
| <i;j++) print("bob");                            | 0 2 12 13 18 19 23          | Reduce(9)   |
| <i;j++) print("bob");                            | 0 2 12 13 18 19 20          | Shift(I_21) |
| i;j++) print("bob");                             | 0 2 12 13 18 19 20 21       | Shift(I_23) |
| ;j++) print("bob");                              | 0 2 12 13 18 19 20 21 23    | Reduce(9)   |
| ;j++) print("bob");                              | 0 2 12 13 18 19 20 21 22    | Reduce(7)   |
| ;j++) print("bob");                              | 0 2 12 13 18 19 25          | Shift(I_26) |
| j++) print("bob");                               | 0 2 12 13 18 19 25 26       | Shift(I_29) |
| ++) print("bob");                                | 0 2 12 13 18 19 25 26 29    | Shift(I_30) |
| ) print("bob");                                  | 0 2 12 13 18 19 25 26 29 30 | Reduce(12)  |
| ) print("bob");                                  | 0 2 12 13 18 19 25 26 31    | Shift(I_32) |
| print("bob");                                    | 0 2 12 13 18 19 25 26 31 32 | Reduce(3)   |
| print("bob");                                    | 0 2 2                       | Shift(I_4)  |
| ("bob");                                         | 0 2 2 4                     | Shift(I_5)  |
| "bob");                                          | 0 2 2 4 5                   | Shift(I_6)  |
| bob");                                           | 0 2 2 4 5 6                 | Shift(I_7)  |
| ");                                              | 0 2 2 4 5 6 7               | Shift(I_8)  |
| );                                               | 0 2 2 4 5 6 7 8             | Shift(I_9)  |
| ;                                                | 0 2 2 4 5 6 7 8 9           | Reduce(14)  |
| ;                                                | 0 2 2 10                    | Shift(I_11) |
| $                                                | 0 2 2 10 11                 | Reduce(2)   |
| $                                                | 0 2 3                       | Reduce(1)   |
| $                                                | 0 1                         | Accept      |

Строка принята
