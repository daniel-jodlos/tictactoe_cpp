cmake_minimum_required(VERSION 3.15)
project(tictactoe)

set(CMAKE_CXX_STANDARD 17)
include_directories("./include")
find_library(SOCKPP_LIB sockpp)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB sources "src/*.cpp" "src/**/*.cpp")
file(GLOB includes "include/*.h" "include/**/*.h")


add_library(tictactoelib ${sources} ${includes})
target_link_libraries(tictactoelib ${SOCKPP_LIB} ncurses pthread)

add_executable(tictactoe main.cpp)
target_link_libraries(tictactoe ${SOCKPP_LIB}  tictactoelib)
add_subdirectory(test)

enable_testing()
add_test(AllTests test/tests)
