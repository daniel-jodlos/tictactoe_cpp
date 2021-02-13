//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include "game_screen.h"
#include <game.h>
#include <iostream>
#include <stdexcept>

Game::Game(Player *p1, Player *p2, Board &board) : _board(board), _round(0) {
  _players[0] = p1;
  _players[1] = p2;
}

Game::~Game() {
  delete _players[0];
  delete _players[1];
}

void Game::round() {
  while (true) {
    try {
      const auto move = _players[_round % 2]->playOn();
      _board.put(move, _players[_round % 2]->getPlaysWith());
      _players[(_round + 1) % 2]->onOpponentMove(
          move, _players[_round % 2]->getPlaysWith());
      std::clog << "On opponent move invoked\n";
      break;
    } catch (std::out_of_range &) {
      if (has_frontend) {
        game_screen::talk_to_player(_players[_round % 2]->getPlaysWith(),
                                    "Out of range");
        getch();
      } else {
        std::cerr << "Out of range occured\n";
      }
      continue;
    } catch (std::runtime_error &) {
      if (has_frontend) {
        game_screen::talk_to_player(_players[_round % 2]->getPlaysWith(),
                                    "Taken");
        getch();
      } else {
        std::clog << "Taken occured\n";
      }
      continue;
    }
  }
  _round++;
}

std::size_t Game::getRoundNumber() noexcept { return _round; }

void Game::setHasFrontend(bool n) noexcept { this->has_frontend = n; }

void Game::printBoard() {
  if (this->has_frontend) {
    game_screen::printBoard(_board);
  }
}

void Game::resolve() {
  while (!_board.isFinished()) {
    printBoard();
    round();
  }

  printBoard();
  const Element winner = _board.getWinner();
  for (const auto &player : _players)
    player->onFinish(winner);
}

Player *Game::getPlayer(int i) { return _players[i % 2]; }
