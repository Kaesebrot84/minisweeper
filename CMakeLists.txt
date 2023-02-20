cmake_minimum_required(VERSION 3.2)
project(minisweeper VERSION 0.0.1 LANGUAGES CXX)
SET(CMAKE_BUILD_TYPE Debug)
SET(MSWEEP minisweeper)

file(GLOB_RECURSE TARGET_SRC "tile.h" "tile.cpp" "digital_display.h" "digital_display.cpp" "field.h" "field.cpp" "game.h" "game.cpp")

add_executable(${MSWEEP} main.cpp ${TARGET_SRC})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${MSWEEP} PRIVATE "-lGL -lraylib -lm -lpthread -ldl -lrt -lX11")
endif()