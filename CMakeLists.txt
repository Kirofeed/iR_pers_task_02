# Минимальная версия CMake
cmake_minimum_required(VERSION 3.20)
# Название проекта
project(MyProject LANGUAGES CXX)

# Установка стандарта C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Отключение расширений компилятора

# Добавляем исполняемый файл
# Если проект многофайловый, просто добавляйте файлы в список через пробел
add_executable(${PROJECT_NAME} 
    main.cpp 
    # Дополнительные файлы можно добавлять сюда, например:
    # file1.cpp file2.cpp
)

# Включение всех предупреждений для компилятора
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Дополнительная опция для кросс-платформенной сборки
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Configuring for macOS")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
endif ()
