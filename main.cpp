#include <iostream>
#include <game.h>
#include <remote_game.h>
#include <cstring>
#include <remote_player.h>
#include <ai_player.h>
#include "game_screen.h"
#include <unistd.h>

void print_help();
template <class T> void print_help_on_fail(T value, std::function<bool(T)> predicate);
void do_create_remote_game(int argc, char** args);
void do_local_game(int argc, char** args);
void do_connect_to_remote_game(int argc, char** args);

int main(int argc, char** args) {
    std::srand(std::time(nullptr));
    print_help_on_fail<int>(argc, [&](int a) { return a >= 2; });

    if(std::strcmp(args[1], "create") == 0) {
        do_create_remote_game(argc, args);
    } else if (std::strcmp(args[1], "local") == 0) {
        do_local_game(argc, args);
    } else if (std::strcmp(args[1], "connect") == 0) {
        do_connect_to_remote_game(argc, args);
    } else {
        print_help();
        return -1;
    }

    getch();
    game_screen::cleanup();
}

#define equal(to) [&](auto a) { return a == to; }
#define print_and_exit_if(predicate, msg) if(predicate) { std::puts(msg); std::exit(0); }

void print_help() {
    const char* help_filename = "resources/help.txt";
    execlp("cat", "cat", help_filename, NULL);
}

template <class T> void print_help_on_fail(T value, std::function<bool(T)> predicate) {
    if(!predicate(value)) {
        print_help();
        std::puts("Incorrect number of arguments");
        std::exit(0);
    }
}

#define equal(to) [&](auto a) { return a == to; }
#define print_and_exit_if(predicate, msg) if(predicate) { std::puts(msg); std::exit(0); }

void do_create_remote_game(int argc, char** args) {
    print_help_on_fail<int>(argc, equal(4));
    int size = std::atoi(args[2]);
    print_and_exit_if(size < 3, "Size of the board has to be at least 3");
    int port = std::atoi(args[3]);

    Board b(size);
    std::cout << "Waiting for connection\n";
    Game g = createRemoteGameServer<Player, RemotePlayer>(port, std::rand() % 2 == 0, b);
    std::cout << "Connection established\n";
    game_screen::init(b.size);
    g.resolve();
}

void do_local_game(int argc, char** args) {
    print_help_on_fail<int>(argc, equal(3));
    int size = std::atoi(args[2]);
    print_and_exit_if(size < 3, "Size of the board has to be at least 3");

    Board b(size);
    Game game = Game::create<Player, Player>(b);
    game_screen::init(size);
    game.resolve();
}

void do_connect_to_remote_game(int argc, char** args) {
    print_help_on_fail<int>(argc, equal(4));
    int port = std::atoi(args[3]);
    auto [game, board] = connectToRemoteGame<Player, RemotePlayer>(args[2], port);
    game_screen::init(board->size);
    game.resolve();
}
