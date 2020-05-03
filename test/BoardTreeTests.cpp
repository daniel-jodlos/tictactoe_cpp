//
// Created by danielj on 03.05.2020.
//
#include <gtest/gtest.h>
#include <board.h>
#include <board_tree.h>

class BoardTreeTests : public ::testing::Test {};

TEST_F(BoardTreeTests, testFindChildWithMove) {
    Board b(3);
    b.put({0,0}, O);
    BoardTree bt(b, O);
    ASSERT_EQ(bt.findChildWithMove({1,1}).getMove(), std::make_pair((std::size_t) 1, (std::size_t) 1));
}

TEST_F(BoardTreeTests, testChildrenHaveUniqueMoves) {
    const static std::size_t size = 3;
    Board b(size);
    BoardTree bt(b, X);

    for(std::size_t x = 0 ; x < size; x++)
        for(std::size_t y = 0; y < size; y++) {
            auto predicate = [&x,y](auto c) { return c.getMove() == std::make_pair(x, y); };
            ASSERT_EQ(std::count_if(bt.getChildrenBegin(), bt.getChildrenEnd(), predicate), 1);
        }
}

TEST_F(BoardTreeTests, testFindBestChild) {
    const static std::size_t size = 3;
    Board b(size);
    BoardTree bt(b, X);
    BoardTree best = bt.findChildWithMove(bt.findBestMove());

    auto predicate = [&best](auto c) { return c.getWinPr() > best.getWinPr() || (c.getWinPr() == best.getWinPr() && c.getLossPr() < best.getLossPr()); };
    ASSERT_EQ(std::count_if(bt.getChildrenBegin(), bt.getChildrenEnd(), predicate), 0);
}

TEST_F(BoardTreeTests, testAmountOfChildren) {
    const std::size_t size = 3;
    Board b(size);
    BoardTree bt(b, X);

    ASSERT_EQ(std::distance(bt.getChildrenBegin(), bt.getChildrenEnd()), size*size);
}
