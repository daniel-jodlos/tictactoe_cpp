//
// Created by Daniel Jodłoś on 11.04.2020.
//

#include <remote_player.h>

RemotePlayer::RemotePlayer(Element playsWith, const Board &on, sockpp::stream_socket socket): Player(playsWith, on), _socket(std::move(socket)) {
}

std::pair<std::size_t, std::size_t> RemotePlayer::playOn() {
    std::pair<std::size_t, std::size_t> buffer;
    _socket.read_n(&buffer, sizeof(decltype(buffer)));
    return buffer;
}

void RemotePlayer::onOpponentMove(std::pair<std::size_t, std::size_t> pair, Element element) {
    _socket.write_n(&pair, sizeof(decltype(pair)));
}

