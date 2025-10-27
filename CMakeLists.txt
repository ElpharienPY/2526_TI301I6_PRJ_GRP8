cmake_minimum_required(VERSION 3.30)
project(2526_TI301I6_PRJ_GRP8 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_executable(2526_TI301I6_PRJ_GRP8 src/main.c src/adj_list.c src/export_mermaid.c src/markov_check.c)
