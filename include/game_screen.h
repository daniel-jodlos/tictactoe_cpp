//
// Created by danielj on 13.04.2020.
//

#ifndef TICTACTOE_GAME_SCREEN_H
#define TICTACTOE_GAME_SCREEN_H

#include <ncurses.h>
#include <board.h>
#include <string>

namespace game_screen {
    void init(int size);
    void cleanup();

    void printBoard(const Board& board);
    void talk_to_player(int player, const std::string& msg);
    void print_instructions();
    std::pair<std::size_t, std::size_t> getInput(Element from);
}

#endif //TICTACTOE_GAME_SCREEN_H
