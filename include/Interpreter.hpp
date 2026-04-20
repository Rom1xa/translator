#include "Definitions.hpp"
#include "Errors.hpp"
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

/**
 * Класс, объект которого исполняет код
 */
class Interpreter {
  /**
   * Внутренняя память интерпретаторa
   */
  std::vector<uint64_t> _memory;

  /**
   * Слово, которое надо выводить
   */
  std::string _word;

  int _scope{0}, _ind{0}, cur_scope{0};

  void _execRoutine(const std::unique_ptr<ParseNode> &i);

  static std::unordered_map<std::string,
                            std::function<bool(uint64_t, uint64_t)>>
      comp;

  std::reference_wrapper<std::ostream> _stream;

public:
  /**
   * Обрезает данное дерево разбора, оставляя только необходимую для
   * семантического анализа информацию. Тем самым, получается AST
   *
   * <for-expr> преобразуется следующим образом:
   * ```
   * for
   *  init
   *      NAME i
   *      NUM 0
   *  cond
   *      operand
   *          NAME i
   *      COMP <
   *      operand
   *          NUM 3
   *  action-str
   *      UNARY_OP ++
   *      NAME i
   * ```
   *
   * <cmd> срезается до одного слова WORD, которое необходимо вывести
   */
  std::unique_ptr<ParseNode> shrink(std::unique_ptr<ParseNode> tree);

  Interpreter();
  Interpreter(std::ostream &stream);

  void addSymbols(const std::unique_ptr<ParseNode> &tree);

  /**
   * Исполняет код, представленный в виде AST
   *
   * \warning переданное дерево должно пройти через процедуру
   * Interpreter::shrink
   */
  void execute(const std::unique_ptr<ParseNode> &ast);

  void validate(const std::unique_ptr<ParseNode> &ast);
};
