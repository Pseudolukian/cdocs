cmake_minimum_required(VERSION 3.10)

project(cdocs)

# Устанавливаем стандарт C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исходные файлы из папок
set(SOURCES
    main.cpp
    cdocs_files/constructor.cpp
    cdocs_files/get_files_list.cpp
    cdocs_files/read_file.cpp
    cdocs_files/save_file.cpp
    cdocs_parser/constructor.cpp
    cdocs_parser/vars_from_txt.cpp
    cdocs_parser/vars_in_docs.cpp
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Добавляем директории с заголовочными файлами
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/cdocs_files
    ${CMAKE_SOURCE_DIR}/cdocs_parser
)

# Копируем vars.txt в директорию сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/vars.txt
        ${CMAKE_BINARY_DIR}/vars.txt
    COMMENT "Copying vars.txt to build directory"
)