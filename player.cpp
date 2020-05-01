//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include "include/player.h"
#include "game_screen.h"
#include <iostream>

Player::Player(const Element e, const Board& b): _playsWith(e), _onBoard(b) {}

std::pair<std::size_t, std::size_t> Player::playOn() {
//    std::size_t x,y;
//    std::cout << "Player " << _playsWith << ": " << std::flush;
//    std::cin >> x >> y; // TODO: implement integration with GUI
//    return std::make_pair(x,y);
    return game_screen::getInput(this->getPlaysWith());
}

void Player::onFinish(bool result) const {
    std::cout << "Player " << _playsWith << ' ' << (result ? "has" : "hasn't") << " won\n";
    // TODO: Implement GUI integration
}

Element Player::getPlaysWith() const {
    return _playsWith;
}

