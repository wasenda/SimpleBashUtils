#!/bin/bash

# Цвета
GREEN="$(tput setaf 2)"  # Зеленый
RED="$(tput setaf 1)"    # Красный
RESET="$(tput sgr0)"     # Сброс цвета

# Тестовые файлы
TEST_FILES=("test.txt")
MY_EXEC="./s21_cat"
REAL_EXEC="cat"

# Флаги для тестирования
MY_EXEC_FLAGS=("b" "e" "n" "s" "t" "v")
COMBINED_FLAGS=(
    "-b -e"
    "-n -s"
    "-t -v"
    "-b -n"
    "-b -n -e"
    "-s -t -v"
    "-b -e -n -s"
    "-b -e -s"
    "-e -t -v"
)

# Функция для тестирования с флагами
test_with_flags() {
    local file=$1
    local flags=$2
    echo "Testing with flags: $flags on file: $file"
    
    # Выполнение команды и сравнение результатов
    diff --ignore-space-change <($REAL_EXEC $flags $file) <($MY_EXEC $flags $file)
    if [ $? -eq 0 ]; then
        echo "${GREEN}Test passed! VICTORY${RESET}"
    else
        echo "${RED}Test failed!${RESET}"
    fi
}

# Основной цикл по тестовым файлам
for file in "${TEST_FILES[@]}"; do
    echo "Testing with $file file:"
    sleep 1.5
    
    # Тестирование с одиночными флагами
    for flag in "${MY_EXEC_FLAGS[@]}"; do
        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo "Testing with flag: -$flag"
        test_with_flags "$file" "-$flag"
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.1
    done

    # Тестирование с комбинированными флагами
    for combined in "${COMBINED_FLAGS[@]}"; do
        echo "~~~~~~~~~~~~~~~~~~~~~"
        test_with_flags "$file" "$combined"
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.1
    done
done