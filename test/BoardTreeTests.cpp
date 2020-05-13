//
// Created by Daniel Jodłoś on 03.05.2020.
//
#include <gtest/gtest.h>
#include <board.h>
#include <board_tree.h>

class BoardTreeTests : public ::testing::Test {};

const std::size_t size = 3;
Board b(size);
BoardTree bt(b, X);

TEST_F(BoardTreeTests, testFindChildWithMove) {
    for(int x = 0; x < size; x++)
        for(int y = 0; y < size; y++)
            ASSERT_EQ(bt.findChildWithMove({x,y}).getMove(), std::make_pair((std::size_t) x, (std::size_t) y));
}

TEST_F(BoardTreeTests, testChildrenHaveUniqueMoves) {
    for(std::size_t x = 0 ; x < size; x++)
        for(std::size_t y = 0; y < size; y++) {
            auto predicate = [&x,y](auto c) { return c.getMove() == std::make_pair(x, y); };
            ASSERT_EQ(std::count_if(bt.getChildrenBegin(), bt.getChildrenEnd(), predicate), 1);
        }
}

TEST_F(BoardTreeTests, testFindBestChild) {
    BoardTree best = bt.findBestMove();

    auto predicate = [&best](auto c) { return c.getWinPr() > best.getWinPr() || (c.getWinPr() == best.getWinPr() && c.getLossPr() < best.getLossPr()); };
    ASSERT_EQ(std::count_if(bt.getChildrenBegin(), bt.getChildrenEnd(), predicate), 0);
}

TEST_F(BoardTreeTests, testAmountOfChildren) {

    ASSERT_EQ(std::distance(bt.getChildrenBegin(), bt.getChildrenEnd()), size*size);
}
