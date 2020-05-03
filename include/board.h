//
// Created by Daniel Jodłoś on 10.04.2020.
//

#ifndef TICTACTOE_BOARD_H
#define TICTACTOE_BOARD_H

#include <vector>

enum Element {
    Empty = 0,
    X = 1,
    O = 2
};

class Board {
private:
    std::vector<std::vector<Element>> _board;
    std::size_t moves = 0;

public:
    const std::size_t size;

    Board(std::size_t);

    [[nodiscard]] Element getWinner() const;
    [[nodiscard]] bool isWon() const;
    [[nodiscard]] bool isFinished() const;

    void put(std::pair<std::size_t, std::size_t> coords, Element what);

    std::vector<Element>::const_iterator operator[](std::size_t) const;
};

#endif //TICTACTOE_BOARD_H
