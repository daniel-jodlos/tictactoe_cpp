Simple TicTacToe implementation with both shared screen and TCP multi-player capabilities, running in the console. So hopefully it could theoretically run on the universities internal network. So... hopefully, theoretically, it should be able to run over the university network.

## Features
- [x] TCP/IP based multi-player
- [x] Shared Screen multi-player
- [x] Ability to handle any size boards
  *victory conditions different to those on the 3x3 grid would need to be aditionally defined*
- [ ] Terminal GUI

## Dependencies
This project requires the following libraries installed in the system, or  otherwise made available to both the CMake script and the compiler:
1. [sockpp](https://github.com/fpagliughi/sockpp) for networking
1. *[Google Test](https://github.com/google/googletest)* for testing **only**
