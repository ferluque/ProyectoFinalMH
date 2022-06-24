cmake_minimum_required(VERSION 3.5)

project(MDD)

include_directories(comun)

add_library(Problem comun/Problem.cpp)
add_library(Solution comun/Solution.cpp)
add_library(bl comun/bl.cpp)

add_executable(BMO src/BMO.cpp)
target_link_libraries(BMO Problem)
target_link_libraries(BMO Solution)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
#target_link_libraries(BMO bl)

add_executable(BMO_memetico src/BMO_memetico.cpp)
target_link_libraries(BMO_memetico Problem)
target_link_libraries(BMO_memetico Solution)
target_link_libraries(BMO_memetico bl)

add_executable(pruebas src/pruebas.cpp)
target_link_libraries(pruebas Problem)
target_link_libraries(pruebas Solution)
target_link_libraries(pruebas bl)