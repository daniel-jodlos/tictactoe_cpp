//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include <game.h>
#include <iostream>

Game::Game(std::unique_ptr<Player> p1, std::unique_ptr<Player> p2, Board &board): _board(board), _round(0) {
    _players[0] = std::move(p1);
    _players[1] = std::move(p2);
}

void Game::round() {
    const auto move = _players[_round % 2]->playOn();
    _board.put(move, _players[_round % 2]->getPlaysWith());
    _players[(_round+1)%2]->onOpponentMove(move, _players[_round % 2]->getPlaysWith());
    _round++;
}

void printBoard(const Board &board) {
    auto printLine = [&board]() {
        for(int i = 0; i < 4*board.size + 1; i++)
            std::cout << "#";
        std::cout << '\n';
    };

    auto printRow = [&board](const auto row) {
        std::cout << "#";

        for(std::size_t i = 0; i < board.size; i++)
            std::cout << ' ' << board[row][i] << " #";

        std::cout << "\n";
    };

    printLine();
    for(std::size_t row = 0; row < board.size; row++) printRow(row);
    printLine();
}

void Game::resolve() {
    while(!_board.isWon()){
        printBoard(_board);
        round();
    }

    printBoard(_board);
    const Element winner = _board.getWinner();
    for(const auto& player : _players)
        player->onFinish(player->getPlaysWith() == winner);
}

Player *Game::getPlayer(int i) {
    return _players[i].get();
}