#include <iostream>
#include <game.h>

int main() {
    Board b(3);
    Game g = Game::create<Player, Player>(b);
    g.resolve();
}
