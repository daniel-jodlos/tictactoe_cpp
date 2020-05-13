//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include <game.h>
#include "game_screen.h"

Game::Game(std::unique_ptr<Player> p1, std::unique_ptr<Player> p2, Board &board): _board(board), _round(0) {
    _players[0] = std::move(p1);
    _players[1] = std::move(p2);
}

void Game::round() {
    while (true) {
        try {
            const auto move = _players[_round % 2]->playOn();
            _board.put(move, _players[_round % 2]->getPlaysWith());
            _players[(_round+1)%2]->onOpponentMove(move, _players[_round % 2]->getPlaysWith());
            break;
        } catch (std::out_of_range&) {
            game_screen::talk_to(_players[_round % 2]->getPlaysWith(), "Incorrect");
            getch();
            continue;
        }
    }
    _round++;
}



void Game::resolve() {
    while(!_board.isFinished()){
        game_screen::printBoard(_board);
        round();
    }

    game_screen::printBoard(_board);
    const Element winner = _board.getWinner();
    for(const auto& player : _players)
        player->onFinish(player->getPlaysWith() == winner);
}

Player *Game::getPlayer(int i) {
    return _players[i].get();
}