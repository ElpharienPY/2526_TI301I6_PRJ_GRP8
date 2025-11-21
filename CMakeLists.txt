cmake_minimum_required(VERSION 3.30)
project(2526_TI301I6_PRJ_GRP8 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(graph_part1
        src/main_part1.c       # part 1 executable
        src/adj_list.c
        src/markov_check.c
        src/export_mermaid.c
)

add_executable(graph_part2
        src/main_part2.c       # part 2 executable
        src/adj_list.c
        src/tarjan.c
        src/hasse.c
        src/partition.c
        interface/sdl_test.c
        interface/sdl_weather.c
)

add_executable(part3
        src/main_part3.c      # part 3 executable
        src/matrix.c
        src/adj_list.c
        src/tarjan.c
        src/partition.c
)

# ================= SDL TEST TARGET =================

# Roots of your SDL2 and SDL2_ttf installs
set(SDL2_ROOT      "C:/Users/hdacc/CLionProjects/2526_TI301I6_PRJ_GRP8/modules/SDL2-2.30.12")
set(SDL2_TTF_ROOT  "C:/Users/hdacc/CLionProjects/2526_TI301I6_PRJ_GRP8/modules/SDL2_ttf-2.20.2")

# Include + lib directories
set(SDL2_INCLUDE_DIR     "${SDL2_ROOT}/x86_64-w64-mingw32/include/SDL2")
set(SDL2_LIB_DIR         "${SDL2_ROOT}/x86_64-w64-mingw32/lib")

set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_ROOT}/x86_64-w64-mingw32/include/SDL2")
set(SDL2_TTF_LIB_DIR     "${SDL2_TTF_ROOT}/x86_64-w64-mingw32/lib")

# Add these to the include + link search paths
include_directories(
        include
        ${SDL2_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
)

link_directories(
        ${SDL2_LIB_DIR}
        ${SDL2_TTF_LIB_DIR}
)

# Simple SDL test executable
add_executable(sdl_test
        interface/sdl_test.c
)

target_link_libraries(sdl_test
        SDL2
        SDL2_ttf
)
add_executable(sdl_weather
        interface/sdl_weather.c
        src/adj_list.c
        src/matrix.c
)

target_link_libraries(sdl_weather
        SDL2
        SDL2_ttf
)


