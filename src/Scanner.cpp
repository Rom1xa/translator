#include "Scanner.hpp"
#include "Definitions.hpp"
#include "Errors.hpp"
#include <cctype>

Scanner::Scanner() : _stream(std::cin) {}

Scanner::Scanner(std::istream &stream) : _stream(stream) {};

Token Scanner::next() {
  // Зашли ли мы в блок, отделенный кавычками. Если да, то все символы мы
  // воспринимаем как часть токена WORD_TOKEN
  static bool is_quoted = false;
  auto &raw_stream = _stream.get();

  // Скипаем пустые символы (если мы не quoted)
  if (!is_quoted) {
    for (auto ch = raw_stream.get(); !raw_stream.eof() && std::isspace(ch);
         ch = raw_stream.get())
      ;
    if (!raw_stream.eof())
      raw_stream.unget();
  }

  auto ch = raw_stream.get();

  if (raw_stream.eof() || raw_stream.fail()) {
    return {TokenType::END, "$"};
  }

  if (is_quoted) {
    if (ch == '"') {
      is_quoted = false;
      return {TokenType::QUOTE, "\""};
    }
    raw_stream.unget();
    return getWord();
  } else {
    if (ch == '(') {
      return {TokenType::LPAR, "("};
    }

    else if (ch == ')') {
      return {TokenType::RPAR, ")"};
    }

    else if (ch == '"') {
      is_quoted = true;
      return {TokenType::QUOTE, "\""};
    }

    else if (ch == ';') {
      return {TokenType::SEMICOL, ";"};
    }

    else if (ch == '+' || ch == '-') {
      raw_stream.unget();
      return getOp();
    }

    else if (comp.count(ch)) {
      raw_stream.unget();
      return getCond();
    }

    else if (std::isdigit(ch)) {
      raw_stream.unget();
      return getNum();
    }

    else if (std::isalpha(ch) || ch == '_') {
      raw_stream.unget();
      return getName();
    }
  }

  throw LexicalError(ch);
}

Token Scanner::getCond() {
  auto &raw_stream = _stream.get();

  // Терминалы cond в два символа
  std::unordered_set<std::string> cond_terms = {
      "<=", ">=", "!=", "==", "&&", "||"};

  Lexem l;
  l.push_back(raw_stream.get());

  {
    auto c = raw_stream.get();
    if (!raw_stream.eof())
      l.push_back(c);
  }

  if (cond_terms.count(l)) {
    return {TokenType::COMP, l};
  }
  raw_stream.unget();

  if (l[0] == '>' || l[0] == '<') {
    return {TokenType::COMP, std::string(1, l[0])};
  } else if (l[0] == '=') {
    return {TokenType::ASSIGN_OP, "="};
  }

  throw LexicalError(l);
}

Token Scanner::getNum() {
  auto &r = _stream.get();
  // Просто извлекаем цифры, пока мы не встретим не-цифру
  Lexem l;
  for (auto ch = r.get(); std::isdigit(ch) && !r.eof(); ch = r.get())
    l += ch;

  if (!r.eof())
    r.unget();

  if (l.size() > 1 && l[0] == 0)
    throw LexicalError(l);

  return {TokenType::NUM, l};
}

Token Scanner::getName() {
  auto &r = _stream.get();
  // Просто извлекаем цифры, пока мы не встретим не-цифру
  Lexem l;
  for (auto ch = r.get(); (std::isalnum(ch) || ch == '_') && !r.eof();
       ch = r.get())
    l += ch;

  if (!r.eof())
    r.unget();

  if (l == "print")
    return {TokenType::Print, "print"};
  if (l == "for")
    return {TokenType::For, "for"};

  return {TokenType::NAME, l};
}

Token Scanner::getOp() {
  auto &raw_stream = _stream.get();

  Lexem l;
  l.push_back(raw_stream.get());
  {
    auto c = raw_stream.get();
    if (!raw_stream.eof())
      l.push_back(c);
  }

  if (l == "++" || l == "--")
    return {TokenType::UNARY_OP, l};
  throw LexicalError(l);
}

Token Scanner::getWord() {
  auto &r = _stream.get();
  // Просто извлекаем цифры, пока мы не встретим не-цифру
  Lexem l;
  for (auto ch = r.get(); ch != '"' && !r.eof(); ch = r.get())
    l += ch;

  if (!r.eof())
    r.unget();
  return {TokenType::WORD, l};
}

void Scanner::putback(Token t) {
  for (auto i = t.second.rbegin(); i != t.second.rend(); ++i) {
    _stream.get().putback(*i);
  }
}
