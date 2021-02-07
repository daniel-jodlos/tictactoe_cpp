Simple TicTacToe implementation with both shared screen and TCP multi-player capabilities, running in the console. So hopefully it could theoretically run on the universities internal network. So... hopefully, theoretically, it should be able to run over the university network.

## Features
- [x] TCP/IP based multi-player
- [x] Shared Screen multi-player
- [x] Ability to handle any size of board greater or equal to

  *victory conditions different to those on the 3x3 grid would need to be additionally defined*
- [x] Terminal UI

## Future feautures
On this branch, I am implementing a websocket service, so that the application can work as a backend server for a React application

## Dependencies
This project requires the following libraries installed in the system, or  otherwise made available to both the CMake script and the compiler:
1. [sockpp](https://github.com/fpagliughi/sockpp) for networking
1. [websockpp](https://github.com/zaphoyd/websocketpp) for websocket. *This library requires Boost libraries, but for some reason doesn't seem to state it*
1. ncurses for terminal UI
1. *[Google Test](https://github.com/google/googletest)* for testing **only**
