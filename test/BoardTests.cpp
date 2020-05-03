//
// Created by Daniel Jodłoś on 10.04.2020.
//
#include <gtest/gtest.h>
#include <board.h>

class BoardTests : public ::testing::Test {};

TEST_F(BoardTests, testInsert) {
    Board b(3);
    ASSERT_THROW(b.put({4,0}, X), std::out_of_range);
    ASSERT_THROW(b.put({0,4}, X), std::out_of_range);

    b.put({0,0},X);
    ASSERT_EQ(b[0][0], X);
}

TEST_F(BoardTests, testInsertIsOneWriteOnly) {
    Board b(1);
    ASSERT_NO_THROW(b.put({0,0}, X));
    ASSERT_THROW(b.put({0,0}, O), std::runtime_error);
}

TEST_F(BoardTests, testWinConditions) {
    for(Element x : {X, O}) {
        Board b(3);
        ASSERT_FALSE(b.isWon());
        b.put({0, 0}, x);
        b.put({0, 1}, x);
        b.put({0, 2}, x);
        ASSERT_TRUE(b.isWon());
        ASSERT_EQ(b.getWinner(), x);

        Board b2(3);
        ASSERT_FALSE(b2.isWon());
        for (int i = 0; i < 3; i++)
            b2.put({i, i}, x);
        ASSERT_EQ(b2.getWinner(), x);

        Board b3(3);
        ASSERT_FALSE(b3.isWon());
        b3.put({0, 2}, x);
        b3.put({1, 1}, x);
        b3.put({2, 0}, x);
        ASSERT_EQ(b3.getWinner(), x);
    }
}

TEST_F(BoardTests, testDraw) {
    Element drawnBoard[3][3] = {
            {X, O, X},
            {O, X, O},
            {O, X, O}
    };

    Board b(3);

    for(int x = 0; x < 3; x++)
        for(int y = 0; y < 3; y++)
            b.put({x,y}, drawnBoard[x][y]);

    ASSERT_TRUE(b.isFinished());
    ASSERT_EQ(b.getWinner(), Empty);
}
