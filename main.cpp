#include <iostream>
#include <game.h>
#include <remote_game.h>
#include <cstring>
#include <remote_player.h>

int main(int argc, char** args) {
    if(std::strcmp(args[1], "create") == 0) {
        int port = std::atoi(args[2]);

        Board b(3);
        Game g = createRemoteGameServer<Player, RemotePlayer>(port, true, b);
        std::cout << "Connection estabilished\n";
        g.resolve();
    } else {
        int port = std::atoi(args[2]);
        auto [game, board] = connectToRemoteGame<Player, RemotePlayer>(args[1], port);
        game.resolve();
    }

    std::cout << "Game resolved\n";
}
