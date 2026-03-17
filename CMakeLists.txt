cmake_minimum_required(VERSION 3.10)
project(SnakeGame VERSION 1.0)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление пути к заголовочным файлам SFML
include_directories(include)
include_directories(/opt/homebrew/include)

# Добавление пути к библиотекам SFML
link_directories(/opt/homebrew/lib)

# Поиск и подключение SFML 3.0
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Добавление всех файлов исходников
file(GLOB_RECURSE SOURCES src/*.cpp)

# Создание исполняемого файла
add_executable(snake ${SOURCES})

# Линковка с библиотеками SFML
target_link_libraries(snake sfml-graphics sfml-window sfml-system)

# Копирование директории assets в директорию сборки
add_custom_command(TARGET snake POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:snake>/assets)

# Дополнительные флаги компилятора
if(MSVC)
    target_compile_options(snake PRIVATE /W4)
else()
    target_compile_options(snake PRIVATE -Wall -Wextra -pedantic)
endif()

# Вывод информации о проекте
message(STATUS "Проект: ${PROJECT_NAME} версия ${PROJECT_VERSION}")
message(STATUS "Исходники: ${SOURCES}")
message(STATUS "Используется SFML 3.0")
