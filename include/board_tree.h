//
// Created by Daniel Jodłoś on 01.05.2020.
//

#ifndef TICTACTOE_BOARD_TREE_H
#define TICTACTOE_BOARD_TREE_H


#include "board.h"
#include <tuple>

class BoardTree : public Board {
private:
    /**
     * List of children accessible from the Board
     */
    std::vector<BoardTree> children;

    /**
     * Probability of interested party winning
     */
    double win_probability;

    /**
     * Probability of interested party loosing
     */
    double loss_probability;

    /**
     * Element whose win is counted towards a win, and a loss towards a loss
     */
    Element interested_party;

    /**
     * Generates the rest of the tree, unless the game is complete
     */
    void _generate();
    std::tuple<std::size_t, std::size_t, std::size_t> count_possibilities();
public:
    explicit BoardTree(Board &, Element success);
    explicit BoardTree(Board&, std::pair<std::size_t, std::size_t> where, Element success);

    /**
     * Traverses the tree
     * @param where
     * @param what
     */
    void put(std::pair<std::size_t, std::size_t> where, Element what) override;
};


#endif //TICTACTOE_BOARD_TREE_H
