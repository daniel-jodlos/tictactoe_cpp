//
// Created by Daniel Jodłoś on 10.04.2020.
//

#ifndef TICTACTOE_PLAYER_H
#define TICTACTOE_PLAYER_H


#include "board.h"

class Player {
private:


protected:
    const Element _playsWith;
    const Board& _onBoard;

public:
    Player(Element, const Board&);

    [[nodiscard]] Element getPlaysWith() const;

    /**
     * Makes a decision where to put it's element
     * @return std::pair<std::size_t, std::size_t> - Coordinates where element will be put
     */
    virtual std::pair<std::size_t, std::size_t> playOn();

    virtual void onOpponentMove(std::pair<std::size_t, std::size_t>, Element){};

    virtual void onFinish(Element result) const;

};


#endif //TICTACTOE_PLAYER_H
