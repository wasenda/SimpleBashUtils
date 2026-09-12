#!/bin/bash

# Цвета
GREEN="$(tput setaf 2)"  # Зеленый
RED="$(tput setaf 1)"    # Красный
RESET="$(tput sgr0)"     # Сброс цвета

# Тестовые файлы
TEST_FILES=("test.txt" "test2.txt")
MY_EXEC="./s21_grep"
REAL_EXEC="grep"

# Флаги для тестирования
MY_FLAGS=(
    "-e apple"
    "-i apple"
    "-v ginger"
    "-c rain"
    "-l apple"
    "-n apple"
    "-h rat" 
    "-s -e banana"  # Пример с флагами
)

# Комбинации флагов для более сложных тестов
COMBINED_FLAGS=(
    "-i -e apple"
    "-v -n ginger"
    "-c -l rain"
    "-h -n -e banana"
)

# Функция для тестирования с флагами
test_with_flags() {
    local file=$1
    local flags=$2
    echo "Testing with flags: $flags on file: $file"
    
    # Выполнение команды и сравнение результатов
    diff --ignore-space-change <($REAL_EXEC $flags "$file") <($MY_EXEC $flags "$file")
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
    for flag in "${MY_FLAGS[@]}"; do
        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo "Testing with flag: $flag"
        test_with_flags "$file" "$flag"
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.1
    done

    # Тестирование с комбинированными флагами
    for combined_flag in "${COMBINED_FLAGS[@]}"; do
        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo "Testing with combined flags: $combined_flag"
        test_with_flags "$file" "$combined_flag"
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.1
    done

done
