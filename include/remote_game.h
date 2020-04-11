//
// Created by danielj on 11.04.2020.
//

#ifndef TICTACTOE_REMOTE_GAME_H
#define TICTACTOE_REMOTE_GAME_H

#include <game.h>
#include <remote_game.h>
#include <sockpp/tcp_socket.h>
#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>

namespace sockpp {
class ConnectionError : public std::runtime_error {
public:
    ConnectionError(): std::runtime_error("Failed to connect to server") {}
};
}

template<class LocalPlayer, class RemotePlayer> Game createRemoteGameServer(int port, bool order, Board& board) {
    sockpp::tcp_acceptor acceptor(port);
    sockpp::tcp_socket socket = acceptor.accept();

    const bool remote_order = !order;
    socket.write_n(&remote_order,sizeof(bool));
    socket.write_n(&board.size, sizeof(decltype(board.size)));

    std::unique_ptr<Player> p1 = std::make_unique<LocalPlayer>(order ? X : O, board);
    std::unique_ptr<Player> p2 = std::make_unique<RemotePlayer>(order ? O : X, board, std::move(socket));

    if(order) std::swap(p1,p2);
    return Game(std::move(p1),std::move(p2),board);
}

template<class LocalPlayer, class RemotePlayer> std::tuple<Game, Board> connectToRemoteGame(std::string addr, int port) {
    sockpp::tcp_connector conn;

    if(!conn.connect(sockpp::inet_address(addr, port))) throw sockpp::ConnectionError();

    bool order;
    conn.read_n(&order, sizeof(bool));

    std::size_t boardSize;
    conn.read_n(&boardSize, sizeof(std::size_t));
    Board board(boardSize);

    std::unique_ptr<Player> p1 = std::make_unique<LocalPlayer>(order ? X : O, board);
    std::unique_ptr<Player> p2 = std::make_unique<RemotePlayer>(order ? O : X, board, std::move(conn));

    if(order) std::swap(p1,p2);
    return std::make_tuple(Game(std::move(p1),std::move(p2),board), std::move(board));
}

#endif //TICTACTOE_REMOTE_GAME_H
