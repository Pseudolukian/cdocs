cmake_minimum_required(VERSION 3.10)

project(cdocs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исходные файлы из папок
file(GLOB SOURCES
    main.cpp
    cdocs_files/*.cpp
    cdocs_parser/*.cpp
    cdocs_conf/*.cpp
    cdocs_regex/*.cpp
    cdocs_buffer/*.cpp
    cdocs_log/*.cpp
)

file(GLOB NOBUILD_FILES 
    "cdocs_files/*_nobuild.cpp" 
    "cdocs_parser/*_nobuild.cpp"
)
list(REMOVE_ITEM SOURCES ${NOBUILD_FILES})

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

# Копируем cdocs.cfg в директорию сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/cdocs.cfg
        ${CMAKE_BINARY_DIR}/cdocs.cfg
    COMMENT "Copying cdocs.cfg to build directory"
)