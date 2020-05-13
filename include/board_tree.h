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

    std::pair<std::size_t, std::size_t> _move;

    /**
     * Generates the rest of the tree, unless the game is complete
     */
    void _generate();
    std::tuple<std::size_t, std::size_t, std::size_t> count_possibilities();

public:
    explicit BoardTree(const Board &, Element success);
    explicit BoardTree(Board&, std::pair<std::size_t, std::size_t> where, Element success);

    BoardTree& findChildWithMove(std::pair<std::size_t, std::size_t> move);
    BoardTree& findBestMove();

    [[nodiscard]] std::pair<std::size_t, std::size_t> getMove() const;

    decltype(children)::const_iterator getChildrenBegin() const { return children.begin(); }
    decltype(children)::const_iterator getChildrenEnd() const { return children.end(); }

    double getWinPr() { return win_probability; }
    double getLossPr() { return loss_probability; }

    BoardTree() : Board(0), interested_party(O), win_probability(0), loss_probability(0) {}
};

//class BoardTreePlaceholder : public BoardTree {
//public:
//    BoardTreePlaceholder() = default;
//};

#endif //TICTACTOE_BOARD_TREE_H
