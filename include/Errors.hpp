#pragma once

#include <exception>
#include <string>
#include <string_view>

/**
 * Ошибка при лексическом анализе
 */
class LexicalError : public std::exception {
  std::string _info;

public:
  /**
   * Конструктор лексической ошибки
   *
   * \param lexem Ошибочная лексема
   */
  LexicalError(std::string_view lexem);

  /**
   * Конструктор лексической ошибки
   *
   * \note В С++ ПОЧЕМУ ТО отсутствует очевидный конструктор
   * std::string(char). Поэтому, мы должны отдельным образом обрабатывать
   * ситуацию, когда лексема - это одиночный символ
   *
   * \param lexem Символ
   */
  LexicalError(char lexem);

  /**
   * Содержание ошибки - неправильная лексема
   *
   * \warning Кто сохранит этот указатель куда-нибудь, тот лох. Если этот
   * указатель переживет объект класса, будет висячий указатель
   */
  const char *what() const noexcept override;
  virtual ~LexicalError() = default;
};

/**
 * Заготовка для ошибки синтаксического разбора
 */
class SyntaxError : public std::exception {

  std::string _info;

public:
  SyntaxError(std::string_view what);

  const char *what() const noexcept override;
  virtual ~SyntaxError() = default;
};

/**
 * Заготовка для семантической ошибки
 */
class SemanticError : public std::exception {

  std::string _info;

public:
  SemanticError(std::string_view what);

  const char *what() const noexcept override;
  virtual ~SemanticError() = default;
};
