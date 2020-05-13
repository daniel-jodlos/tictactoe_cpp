//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include "include/player.h"
#include "game_screen.h"
#include <iostream>

Player::Player(const Element e, const Board& b): _playsWith(e), _onBoard(b) {}

std::pair<std::size_t, std::size_t> Player::playOn() {
    return game_screen::getInput(this->getPlaysWith());
}

void Player::onFinish(bool result) const {
    std::cout << "Player " << _playsWith << ' ' << (result ? "has" : "hasn't") << " won\n";
    // TODO: Implement GUI integration
    game_screen::talk_to(getPlaysWith(), result ? "Winner" : "Looser");
}

Element Player::getPlaysWith() const {
    return _playsWith;
}

