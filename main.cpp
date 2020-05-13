#include <iostream>
#include <game.h>
#include <remote_game.h>
#include <cstring>
#include <remote_player.h>
#include <ai_player.h>
#include "game_screen.h"

int main(int argc, char** args) {
    if(std::strcmp(args[1], "create") == 0) {
        int size = std::atoi(args[2]);
        if(size < 3) {
            std::puts("Size has to be at least 3");
            return 0;
        }
        int port = std::atoi(args[3]);

        Board b(size);
        Game g = createRemoteGameServer<Player, RemotePlayer>(port, true, b);
        std::cout << "Connection established\n";
        game_screen::init(b.size);
        g.resolve();
    } else if (std::strcmp(args[1], "local") == 0) {
        int size = std::atoi(args[2]);
        if(size < 3) {
            std::puts("Size has to be at least 3");
            return 0;
        }
        Board b(size);
        Game game = Game::create<Player, Player>(b);
        game_screen::init(size);
        game.resolve();
    } else if (std::strcmp(args[1], "connect") == 0) {
        int port = std::atoi(args[3]);
        auto [game, board] = connectToRemoteGame<Player, RemotePlayer>(args[2], port);
        game_screen::init(board->size);
        game.resolve();
    } else if (std::strcmp(args[1], "ai") == 0) {
        Board b(3);
        Game game = Game::create<Player, AIPlayer>(b);
        game_screen::init(b.size);
        game.resolve();
    } else {
        std::puts("WTF?");
        return -1;
    }

    getch();
    game_screen::cleanup();
}
