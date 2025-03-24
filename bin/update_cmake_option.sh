#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <module_name> <state>"
    echo "Example: $0 module1 ON"
    exit 1
fi

MODULE_NAME=$1
STATE=$2

if [[ "$STATE" != "ON" && "$STATE" != "OFF" ]]; then
    echo "Error: State must be either 'ON' or 'OFF'."
    exit 1
fi

CMAKE_FILE="../CMakeLists.txt"

if [ ! -f "$CMAKE_FILE" ]; then
    echo "Error: File '$CMAKE_FILE' not found."
    exit 1
fi

# Сохраняем оригинальный регистр модуля
OPTION_LINE="option(ENABLE_${MODULE_NAME} \"Enable ${MODULE_NAME}\" ${STATE})"

# Функция для проверки существования вызова модуля
function module_call_exists() {
    grep -q "add_module_if_enabled(${MODULE_NAME})" "$CMAKE_FILE"
}

# Функция для добавления вызова модуля
function add_module_call() {
    # Ищем последний вызов add_module_if_enabled
    last_line=$(grep -n "add_module_if_enabled(" "$CMAKE_FILE" | tail -1 | cut -d: -f1)

    if [ -z "$last_line" ]; then
        # Если не найдено ни одного вызова, вставляем после function
        func_line=$(grep -n "function(add_module_if_enabled" "$CMAKE_FILE" | cut -d: -f1)
        if [ -n "$func_line" ]; then
            sed -i "$((func_line + 1))i add_module_if_enabled(${MODULE_NAME})" "$CMAKE_FILE"
        else
            # Если function не найден, вставляем перед add_subdirectory(tests)
            test_line=$(grep -n "add_subdirectory(tests)" "$CMAKE_FILE" | cut -d: -f1)
            sed -i "${test_line}i add_module_if_enabled(${MODULE_NAME})" "$CMAKE_FILE"
        fi
    else
        # Вставляем после последнего вызова
        sed -i "$((last_line + 1))i add_module_if_enabled(${MODULE_NAME})" "$CMAKE_FILE"
    fi
}

# Функция для удаления вызова модуля
function remove_module_call() {
    sed -i "/add_module_if_enabled(${MODULE_NAME})/d" "$CMAKE_FILE"
}

# Обновляем option
if grep -q "option(ENABLE_${MODULE_NAME} " "$CMAKE_FILE"; then
    sed -i "s|option(ENABLE_${MODULE_NAME} .*$|${OPTION_LINE}|" "$CMAKE_FILE"
    echo "Updated option for module: ${MODULE_NAME} to state: ${STATE}"
else
    # Вставляем без пустой строки после enable_testing()
    sed -i "/enable_testing()/a ${OPTION_LINE}" "$CMAKE_FILE"
    echo "Added new option for module: ${MODULE_NAME} with state: ${STATE}"
fi

# Обрабатываем вызов модуля
if [[ "$STATE" == "ON" ]]; then
    if ! module_call_exists; then
        add_module_call
        echo "Added module call for ${MODULE_NAME}"
    fi
else
    if module_call_exists; then
        remove_module_call
        echo "Removed module call for ${MODULE_NAME}"
    fi
fi