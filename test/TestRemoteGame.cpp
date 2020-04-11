//
// Created by Daniel Jodłoś on 11.04.2020.
//

#include <gtest/gtest.h>
#include <remote_game.h>
#include <remote_player.h>
#include <stdexcept>
#include <chrono>
#include <thread>

using std::chrono_literals::operator""ms;

class RemoteGame : public ::testing::Test {};

#define createServer(port, size,order) { \
    Board b(size); \
    std::thread thr([&b]() { createRemoteGameServer<Player, RemotePlayer>(port,order,b); }); \
    thr.detach(); \
    std::this_thread::sleep_for(1ms); \
}

TEST_F(RemoteGame, testWrongAddrThrow) {
    ASSERT_THROW((connectToRemoteGame<Player, RemotePlayer>("localhost", 2012)), sockpp::ConnectionError);
    ASSERT_NO_THROW(createServer(1234,3,true));
    ASSERT_NO_THROW((connectToRemoteGame<Player, RemotePlayer>("localhost", 1234)));
}

TEST_F(RemoteGame, testBoardSizeIsCorrect) {
    const int port = 1522;
    const std::size_t size = 3;
    ASSERT_NO_THROW(createServer(port,size,true));
    auto [_, board] = connectToRemoteGame<Player, RemotePlayer>("localhost", port);

    ASSERT_EQ(board.size, size);
}

TEST_F(RemoteGame, testRemoteOrderIsFlipped) {
    const int port = 1522;
    const std::size_t size = 3;
    ASSERT_NO_THROW(createServer(port,size,true));
    auto [game, _] = connectToRemoteGame<Player, RemotePlayer>("localhost", port);

    ASSERT_EQ(game.getPlayer(0)->getPlaysWith(), O);
    ASSERT_EQ(game.getPlayer(1)->getPlaysWith(), X);
}
