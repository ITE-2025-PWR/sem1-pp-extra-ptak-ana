cmake_minimum_required(VERSION 4.0)
project(extra C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-arch arm64")
add_executable(menu src/matrix.c
        src/game_of_life.c
        src/menu.c
        ananas.h
        src/tower_of_hanoi.c
        src/menu_enum_conversions.c
        src/menu_utility.c
        src/menu_input_parser.c)
