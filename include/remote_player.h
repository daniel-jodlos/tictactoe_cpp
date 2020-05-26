//
// Created by danielj on 10.04.2020.
//

#ifndef TICTACTOE_REMOTE_PLAYER_H
#define TICTACTOE_REMOTE_PLAYER_H


#include "player.h"
#include <sockpp/stream_socket.h>

class RemotePlayer : public Player {
private:
    sockpp::stream_socket _socket;
public:
    RemotePlayer(Element playsWith, const Board& on, sockpp::stream_socket socket);

    /**
     * Receives move of the remote opponent
     * @return Move of the remote opponent
     */
    [[nodiscard]] std::pair<std::size_t, std::size_t> playOn() override;

    /**
     * Sends information about your move to the opponent, who will receive it through playOn. Invoked by Game class
     * @param Coordinates of your move
     * @param Element you played with
     */
    void onOpponentMove(std::pair<std::size_t, std::size_t> pair, Element element) override;
};
#endif //TICTACTOE_REMOTE_PLAYER_H
