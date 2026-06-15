#!/usr/bin/env bash

set -u

BIN=build/forlang
TEST_DIR=tests
fail=0

run_case() {
  local file="$1"
  local expect="$2"
  echo "----------------------------------------------------------------"
  echo ">>> $file (ожидается: $expect)"
  "$BIN" "$file"
  local rc=$?
  if [ "$expect" = "pass" ] && [ "$rc" -ne 0 ]; then
    echo "ПРОВАЛ: ожидался успех, код возврата $rc"
    fail=1
  elif [ "$expect" = "fail" ] && [ "$rc" -eq 0 ]; then
    echo "ПРОВАЛ: ожидалась ошибка, но код возврата 0"
    fail=1
  else
    echo "OK (return code: $rc)"
  fi
}

run_case "$TEST_DIR/positive_basic.txt" pass
run_case "$TEST_DIR/positive_nested.txt" pass
run_case "$TEST_DIR/positive_prefix_unary.txt" pass
run_case "$TEST_DIR/positive_name_name_init.txt" pass

run_case "$TEST_DIR/negative_missing_semicolon.txt" fail
run_case "$TEST_DIR/negative_missing_paren.txt" fail
run_case "$TEST_DIR/negative_bare_print.txt" fail
run_case "$TEST_DIR/negative_undefined_var.txt" fail

echo "================================================================"
if [ "$fail" -eq 0 ]; then
  echo "ВСЕ ТЕСТЫ ПРОЙДЕНЫ"
else
  echo "ЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ"
fi
exit "$fail"
