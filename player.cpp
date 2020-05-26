//
// Created by Daniel Jodłoś on 10.04.2020.
//

#include "include/player.h"
#include "game_screen.h"

Player::Player(const Element e, const Board& b): _playsWith(e), _onBoard(b) {}

std::pair<std::size_t, std::size_t> Player::playOn() {
    return game_screen::getInput(this->getPlaysWith());
}

void Player::onFinish(Element result) const {
    // TODO: Implement GUI integration
    if(result == Empty) game_screen::talk_to_player(getPlaysWith(), "Draw");
    else game_screen::talk_to_player(getPlaysWith(), result == getPlaysWith() ? "Winner" : "Looser");
}

Element Player::getPlaysWith() const {
    return _playsWith;
}

