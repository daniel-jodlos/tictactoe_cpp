//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include <cstddef>
#include <stdexcept>
#include "include/board.h"

Element Board::getVerticalWinner() const {
    for(std::size_t col = 0; col < size; col++) {
        Element t = _board[0][col];
        for (std::size_t row = 1; row < size && t != Empty; row++) {
            if (t != _board[row][col])
                t = Empty;
        }
        if(t != Empty) return t;
    }

    return Empty;
}

Element Board::getHorizontalWinner() const {
    for(std::size_t row = 0; row < size; row++) {
        Element t = _board[row][0];
        for (std::size_t col = 1; col < size && t != Empty; col++) {
            if (t != _board[row][col])
                t = Empty;
        }
        if(t != Empty) return t;
    }

    return Empty;
}

Element Board::getDiagonalWinner() const {
    Element t = _board[0][0];
    for(std::size_t i = 1; i < size && t != Empty; i++) {
        if(t != _board[i][i]) t = Empty;
    }

    if(t == Empty) t = _board[0][size-1];
    else return t;

    for(std::size_t i = 1; i < size && t != Empty; i++)
        if(t != _board[i][size-1-i]) t = Empty;

    return t;
}

Element Board::getWinner() const {
    Element winner = getVerticalWinner();
    #define conditional_call(function) if(winner != Empty) return winner; else winner = function()

    conditional_call(getHorizontalWinner);
    conditional_call(getDiagonalWinner);

    return winner;
}

bool Board::isWon() const {
    return getWinner() != Empty;
}

std::vector<Element>::const_iterator Board::operator[](const std::size_t i) const {
    if (i >= size) throw std::out_of_range("Index out of range");
    else return _board[i].cbegin();
}

void Board::put(std::pair<std::size_t, std::size_t> coords, Element what) {
    if(coords.first >= size || coords.second >= size) throw std::out_of_range("Index out of range");
    else if (_board[coords.first][coords.second] != Empty) throw std::runtime_error("Cannot write to the cell for the second time");
    else {
        _board[coords.first][coords.second] = what;
        moves++;
    }
}

Board::Board(std::size_t size): size(size) {
    _board.resize(size);

    for(int x = 0; x < size; x++) {
        _board[x].resize(size);
        for (int y = 0; y < size; y++)
            _board[x][y] = Empty;
    }
}

bool Board::isFinished() const {
    return moves == size*size || isWon();
}

