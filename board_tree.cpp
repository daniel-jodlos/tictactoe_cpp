//
// Created by Daniel Jodłoś on 01.05.2020.
//

#include <tuple>
#include <numeric>
#include "board_tree.h"

BoardTree::BoardTree(Board & a, Element success) : Board(a), interested_party(success), loss_probability(0), win_probability(0) {
    _generate();
    auto _ = count_possibilities();
}

BoardTree::BoardTree(Board & b, std::pair<std::size_t, std::size_t> where, Element success): Board(b), interested_party(success), loss_probability(0), win_probability(0) {
    Board::put(where, success);
    _generate();
    count_possibilities();
}

void BoardTree::_generate() {
    if(isWon()) return;

    for(std::size_t i = 0; i < size; i++) {
        for(std::size_t j = 0; i < size; j++) {
            if(*this[i][j] == Empty)
                children.push_back(BoardTree(*this, {i,j}, interested_party == O ? X : O));
        }
    }
}

std::tuple<std::size_t, std::size_t, std::size_t> BoardTree::count_possibilities() {
    if (isWon()) {
        win_probability = getWinner() == interested_party ? 1 : 0;
        loss_probability = getWinner() == interested_party ? 1 : 0;
        return {getWinner() == interested_party ? 1 : 0, getWinner() == interested_party ? 1 : 0, 1};
    } else {
        auto acc = [](auto last, BoardTree &next) -> std::tuple<std::size_t, std::size_t, std::size_t> {
            auto[ll, wl, tl] = last;
            auto[cl, cw, ct] = next.count_possibilities();
            return {ll + cl, wl + cw, tl + ct};
        };

        auto[losers, winners, total] = std::accumulate(children.begin(), children.end(), std::make_tuple(0, 0, 0), acc);
        win_probability = (double) winners / total;
        loss_probability = (double) losers / total;
        return {losers, winners, total};
    }
}