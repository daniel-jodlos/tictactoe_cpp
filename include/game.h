//
// Created by Daniel Jodłoś on 10.04.2020.
//

#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H


#include <memory>
#include "player.h"

class Game {
protected:
    constexpr const static Element elements[2] = {X, O};

    std::unique_ptr<Player> _players[2];
    Board& _board;
    std::size_t _round;

public:
    Game(std::unique_ptr<Player> p1, std::unique_ptr<Player> p2, Board& board);
    template<class A, class B> static Game create(Board&);

    void round();
    void resolve();

    Player* getPlayer(int);
};

template<class A, class B>
Game Game::create(Board& board) {
    return Game(std::make_unique<A>(elements[0], board), std::make_unique<B>(elements[1], board), board);
}


#endif //TICTACTOE_GAME_H
