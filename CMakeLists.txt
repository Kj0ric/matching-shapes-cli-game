cmake_minimum_required(VERSION 3.29)
project(matching_shapes_cli_game)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(matching_shapes_cli_game
    main.cpp
    main.h)
