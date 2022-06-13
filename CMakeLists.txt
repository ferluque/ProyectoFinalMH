cmake_minimum_required(VERSION 3.5)

project(MDD)

include_directories(comun)

add_library(Problem comun/Problem.cpp)
add_library(Solution comun/Solution.cpp)
add_library(bl comun/bl.cpp)

add_executable(ES src/ES.cpp)
target_link_libraries(ES Problem)
target_link_libraries(ES Solution)

add_executable(BMB src/BMB.cpp)
target_link_libraries(BMB Problem)
target_link_libraries(BMB Solution)
target_link_libraries(BMB bl)

add_executable(ILS src/ILS.cpp)
target_link_libraries(ILS Problem)
target_link_libraries(ILS Solution)
target_link_libraries(ILS bl)

add_executable(ILS-ES src/ILS-ES.cpp)
target_link_libraries(ILS-ES Problem)
target_link_libraries(ILS-ES Solution)
target_link_libraries(ILS-ES bl)

add_executable(pruebas src/pruebas.cpp)
target_link_libraries(pruebas Problem)
target_link_libraries(pruebas Solution)
target_link_libraries(pruebas bl)