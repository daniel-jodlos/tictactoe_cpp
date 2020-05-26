//
// Created by Daniel Jodłoś on 13.04.2020.
//

#include "game_screen.h"
#include <ncurses.h>
#include <map>
#include <fstream>
#include <set>

WINDOW *boardWindow;
WINDOW *players[2];
const int playerH = 5;
const int playerW = 30;

using namespace game_screen;

namespace game_screen {

    void setupPlayerWindow(int i, int y, int x) {
        WINDOW* t = newwin(playerH,playerW,y,x);
        box(t, 0, 0);
        wmove(t, 0, (playerW-10)/2);
        wprintw(t, " Player %d ", i);
        wrefresh(t);
        delwin(t);

        players[i-1] = newwin(1, playerW-5,y+2,x+2);
        wclear(players[i-1]);
        wrefresh(players[i-1]);
        refresh();
    }

    void init(const int size) {
        initscr(); // Init ncurses
        cbreak(); // Terminate on CTRL-C
        noecho(); // Do not print user's input by default

        print_instructions();

        const int height = 2*size + 1;
        const int width = 4*size + 1;
        const int top_margin = 5;
        const int s_width = getmaxx(stdscr);
        boardWindow = newwin(height, width, top_margin, (s_width - width)/2);
        refresh();
        box(boardWindow,0,0);
        wrefresh(boardWindow);

        const int player_window_y = 2*top_margin + height;
        setupPlayerWindow(1, player_window_y, (s_width - 2 * playerW) / 3);
        setupPlayerWindow(2, player_window_y, 2 * ((s_width - 2 * playerW) / 3) + playerW);

        // Print empty board
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
            }

            if(row != board.size-1) {
                wmove(boardWindow, 2*row+2, 1);
                for (std::size_t i = 0; i < board.size - 1; i++)
                    wprintw(boardWindow, "---+");
                wprintw(boardWindow, "---");
            }
        };

        for(std::size_t row = 0; row < board.size; row++) printRow(row);
        wrefresh(boardWindow);
    }

    std::pair<std::size_t, std::size_t> getInput(Element from) {
        WINDOW* target = players[(int) from - 1];
        wclear(target);
        echo();

        std::size_t x;
        std::size_t y;

        wscanw(target, "%ld,%ld\n",&x,&y);

        noecho();
        wclear(target);

        return std::make_pair(x,y);
    }

    void cleanup() {
        endwin();
    }

    void talk_to_player(const int player, const std::string& msg) {
        WINDOW * target = players[player-1];
        wclear(target);
        wprintw(target, msg.c_str());
        wrefresh(target);
    }

    WINDOW* prepare_titled_window(WINDOW* target, const std::string title) {
        box(target, 0, 0);
        const int width = getmaxx(target);

        wmove(target, 0, (width - title.size())/2);
        wprintw(target, title.c_str());
        wrefresh(target);
        refresh();

        return nullptr;
    }

    std::vector<std::string> read_to_lines(std::ifstream& file, std::size_t maxline_length = std::numeric_limits<std::size_t>::infinity()) {
        std::vector<std::string> contents;
        std::string buffer;

        while(!file.eof()) {
            std::getline(file, buffer);

            while(buffer.size() > maxline_length) {
                std::string part = buffer.substr(0, maxline_length);
                buffer = buffer.substr(maxline_length);
                contents.push_back(part);
            }

            contents.push_back(buffer);
        }

        return std::move(contents);
    }

    void print_instructions() {
        const char* instructions_file = "resources/instruction.txt";
        std::ifstream file;
        file.open(instructions_file);

        if(!file.is_open()) {
            throw std::runtime_error("Failed to open file " + std::string(instructions_file));
        }

        clear();
        int scrwidth = getmaxx(stdscr);
        int scrheight = getmaxy(stdscr);
        const int x_margin = scrwidth / 10; // 10%
        const int y_margin = scrheight / 10; // 10%
        const int win_width = scrwidth - 2*x_margin;

        std::vector<std::string> contents = read_to_lines(file, win_width - 4);

        WINDOW* instructions = newwin(contents.size()+2, win_width, y_margin, x_margin);
        refresh();
        prepare_titled_window(instructions, " Instrukcja ");

        for(int line = 1; line <= contents.size(); line++) {
            wmove(instructions, line, 2);
            wprintw(instructions, contents[line-1].c_str());
        }
        wrefresh(instructions);

        getch();
        wclear(instructions);
        delwin(instructions);
        clear();
    }
}