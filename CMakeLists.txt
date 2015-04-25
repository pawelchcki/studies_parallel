project(lab_1)
find_package(OpenMP)


set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -fopenmp -O0 -std=gnu11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -fopenmp -O0")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp -O0")

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(main main.cpp)
add_executable(part_1 part_1.cpp)
add_executable(part_2 part2.cpp)
add_executable(part_2_basic part2_basic.cpp)
add_executable(part_2_reduction part2_reduction.cpp)
add_executable(part_2_line_length part2_line_lenght.cpp)
add_executable(internet_test internet_test.c)

set(CMAKE_BUILD_TYPE Release)
