cmake_minimum_required(VERSION 3.20)
project(lexer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Strict warnings
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Optimisation for 50 KB constraint
set(CMAKE_C_FLAGS_RELEASE "-Os")

add_executable(lexer
        src/main.c
        src/dfa.c
        src/keywords.c
        src/lexer.c
        src/logger.c
)

target_include_directories(lexer PRIVATE src)