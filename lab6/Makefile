BUILD_DIR = build
SRC_DIR   = src
TEST_DIR  = tests

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(SRC_DIR) -I$(BUILD_DIR)

TARGET = $(BUILD_DIR)/forlang

# Анализаторы
PARSER_CPP = $(BUILD_DIR)/parser.tab.cpp
PARSER_HPP = $(BUILD_DIR)/parser.tab.hpp
LEXER_CPP  = $(BUILD_DIR)/lex.yy.cpp

HAND_SRCS = $(SRC_DIR)/ast.cpp $(SRC_DIR)/interpreter.cpp $(SRC_DIR)/main.cpp

.PHONY: all test clean docker-build docker-test

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PARSER_CPP) $(PARSER_HPP): $(SRC_DIR)/parser.y | $(BUILD_DIR)
	bison -d -o $(PARSER_CPP) $(SRC_DIR)/parser.y

$(LEXER_CPP): $(SRC_DIR)/lexer.l $(PARSER_HPP) | $(BUILD_DIR)
	flex -o $(LEXER_CPP) $(SRC_DIR)/lexer.l

$(TARGET): $(PARSER_CPP) $(LEXER_CPP) $(HAND_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PARSER_CPP) $(LEXER_CPP) $(HAND_SRCS) -o $(TARGET)

test: all
	@bash run_tests.sh

clean:
	rm -rf $(BUILD_DIR)

docker-build:
	docker build -t forlang-lab6 .

docker-test: docker-build
	docker run --rm forlang-lab6 make test
