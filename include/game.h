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

  Player *_players[2];
  Board &_board;
  std::size_t _round;
  bool has_frontend = true;

  void printBoard();

public:
  Game(Player *p1, Player *p2, Board &board);
  ~Game();
  template <class A, class B> static Game create(Board &);

  void round();
  void resolve();
  void setHasFrontend(bool) noexcept;

  Player *getPlayer(int);
};

template <class A, class B> Game Game::create(Board &board) {
  return Game(new A(elements[0], board), new B(elements[1], board), board);
}

#endif // TICTACTOE_GAME_H
