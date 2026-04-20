cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project("translator")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(TESTS "Enable tests" ON)

add_library(sym_table src/SymbolTable.cpp)
target_include_directories(sym_table PRIVATE include)

add_library(errors src/Errors.cpp)
target_include_directories(errors PRIVATE include)

add_library(scanner src/Scanner.cpp)
target_include_directories(scanner PRIVATE include)
target_link_libraries(scanner PRIVATE errors sym_table)

add_library(parser src/Parser.cpp)
target_include_directories(parser PRIVATE include)
target_link_libraries(parser PRIVATE errors)

add_library(interpreter src/Interpreter.cpp)
target_include_directories(interpreter PRIVATE include)
target_link_libraries(interpreter PRIVATE errors sym_table)

add_executable(translator ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(translator PRIVATE include)
target_link_libraries(translator sym_table scanner parser interpreter)

if(TESTS)
  add_executable(tests src/tests.cpp)
  target_include_directories(tests PRIVATE include)
  target_link_libraries(tests sym_table scanner parser interpreter)
endif()

target_compile_options(scanner PUBLIC -g)

