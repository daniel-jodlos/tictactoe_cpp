#include <iostream>
#include <game.h>
#include <remote_game.h>
#include <cstring>
#include <remote_player.h>
#include <ai_player.h>
#include "game_screen.h"

int main(int argc, char** args) {
    if(std::strcmp(args[1], "create") == 0) {
        int port = std::atoi(args[2]);

        Board b(3);
        Game g = createRemoteGameServer<Player, RemotePlayer>(port, true, b);
        std::cout << "Connection established\n";
        game_screen::init(3);
        g.resolve();
    } else if (std::strcmp(args[1], "local") == 0) {
        Board b(3);
        Game game = Game::create<Player, Player>(b);
        game_screen::init(3);
        game.resolve();
    } else if (std::strcmp(args[1], "connect") == 0) {
        int port = std::atoi(args[3]);
        auto result = connectToRemoteGame<Player, RemotePlayer>(args[2], port);
        Board board = reinterpret_cast<Board &&>(std::get<0>(result));
        Game game = reinterpret_cast<Game &&>(std::get<1>(result));
        game_screen::init(3);
        game.resolve();
    } else if (std::strcmp(args[1], "ai") == 0) {
        Board b(3);
        Game game = Game::create<Player, AIPlayer>(b);
        game_screen::init(3);
        game.resolve();
    } else {
        std::puts("WTF?");
        return -1;
    }

    std::cout << "Game resolved\n";
    game_screen::cleanup();
}
