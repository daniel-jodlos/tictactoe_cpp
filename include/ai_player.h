//
// Created by Daniel Jodłoś on 01.05.2020.
//

#ifndef TICTACTOE_AI_PLAYER_H
#define TICTACTOE_AI_PLAYER_H


#include "player.h"
#include "placeholder.h"
#include <board_tree.h>

class AIPlayer : public Player {
protected:
    size_t turn;
    BoardTree tree;

public:
    AIPlayer(Element with, Board& board): Player(with, board), turn(0), tree(Placeholder<BoardTree>()) {}

    std::pair<std::size_t, std::size_t> playOn() override;
    void onOpponentMove(std::pair<std::size_t, std::size_t> pair, Element element) override;
};


#endif //TICTACTOE_AI_PLAYER_H
