//
// Created by danielj on 13.04.2020.
//

#include "game_screen.h"
#include <ncurses.h>
#include <map>
#include <sstream>

WINDOW *boardWindow;
WINDOW *players[2];
int i = -1;
const int playerH = 5;
const int playerW = 30;

using namespace game_screen;

namespace game_screen {

    void setupWindow(int i, int y, int x) {
        WINDOW* t = newwin(playerH,playerW,y,x);
        box(t, 0, 0);
        wmove(t, 0, (playerW-10)/2);
        wprintw(t, " Player %d ", i);
        wrefresh(t);
        delwin(t);

        players[i-1] = newwin(1, playerW-5,y+2,x+2);
        wrefresh(players[i-1]);
        refresh();
    }

    void init(const int size) {
        initscr(); // Init ncurses
        cbreak(); // Terminate on CTRL-C
        noecho(); // Do not print user's input by default

        const int height = 2*size + 1;
        const int width = 4*size + 1;
        boardWindow = newwin(height, width, 5, 10);
        refresh();
        box(boardWindow,0,0);
        wrefresh(boardWindow);

        setupWindow(1, 6, 15 + width);
        setupWindow(2, 15, 15 + width);

        Board randomBoard(size);
        printBoard(randomBoard);
    }

    void printBoard(const Board &board) {
        wmove(boardWindow,1,1);
        static const std::map<Element, char> charMap = {
                {X, 'X'},
                {O, 'O'},
                {Empty, ' '}
        };

        auto printRow = [&board](const auto row) {
            wmove(boardWindow, 2*row+1, 1);

            for(std::size_t i = 0; i < board.size; i++) {
                char element[2] = {charMap.at(board[row][i])};
                wprintw(boardWindow, " %s ", &element);
                if(i != board.size-1) wprintw(boardWindow, "|");
                //std::cout << ' ' << charMap.at(board[row][i]) << " #";
            }

            if(row != board.size-1) {
                wmove(boardWindow, 2*row+2, 1);
                for (std::size_t i = 0; i < board.size - 1; i++)
                    wprintw(boardWindow, "---+");
                wprintw(boardWindow, "---");
            }

//            printLine();
        };

//        printLine();
        for(std::size_t row = 0; row < board.size; row++) printRow(row);
        wrefresh(boardWindow);
    }

    std::pair<std::size_t, std::size_t> getInput(Element from) {
        WINDOW* target = players[(int) from - 1];
        wclear(target);
        echo();

        std::size_t x;
        std::size_t y;

        wscanw(target, "%ld %ld\n",&x,&y);

        noecho();
        wclear(target);

        return std::make_pair(x,y);
    }

    void cleanup() {
        endwin();
    }
}