cmake_minimum_required(VERSION 3.5)

project(MDD)

include_directories(comun)

add_library(Problem comun/Problem.cpp)
add_library(Solution comun/Solution.cpp)
#add_library(bl comun/bl.cpp)

add_executable(BMO src/BMO.cpp)
target_link_libraries(BMO Problem)
target_link_libraries(BMO Solution)
#target_link_libraries(BMO bl)

add_executable(pruebas src/pruebas.cpp)
target_link_libraries(pruebas Problem)
target_link_libraries(pruebas Solution)
#target_link_libraries(pruebas bl)