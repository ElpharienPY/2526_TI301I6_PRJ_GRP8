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
)
