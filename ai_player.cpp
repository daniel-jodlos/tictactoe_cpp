//
// Created by Daniel Jodłoś on 01.05.2020.
//

#include <board_tree.h>
#include "ai_player.h"

std::pair<std::size_t, std::size_t> AIPlayer::playOn() {
    turn++;
    if(turn == 1) {
        if(getPlaysWith() == X) { // first player always plays to the corner
            return {0,0};
        } else { // second player plays to the center, if possible
            return _onBoard[1][1] == Empty ? std::make_pair(1,1) : std::make_pair(0,1);
        }
    }

    if(turn == 2) tree = BoardTree(_onBoard, _playsWith);
    tree = tree.findBestMove();
    return tree.getMove();
}

void AIPlayer::onOpponentMove(std::pair<std::size_t, std::size_t> pair, Element element) {
    if(turn >= 2)
        tree = tree.findChildWithMove(pair);
}
