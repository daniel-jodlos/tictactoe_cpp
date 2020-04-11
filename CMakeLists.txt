cmake_minimum_required(VERSION 3.15)
project(tictactoe)

set(CMAKE_CXX_STANDARD 17)
include_directories("./include")
find_library(SOCKPP_LIB sockpp)

add_library(tictactoelib board.cpp include/board.h player.cpp include/player.h game.cpp include/game.h include/remote_player.h remote_player.cpp include/remote_game.h)
target_link_libraries(tictactoelib ${SOCKPP_LIB})
add_executable(tictactoe main.cpp board.cpp include/board.h player.cpp include/player.h game.cpp include/game.h include/remote_player.h remote_player.cpp include/remote_game.h)
target_link_libraries(tictactoe ${SOCKPP_LIB})
add_subdirectory(test)