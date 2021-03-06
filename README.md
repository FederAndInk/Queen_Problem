# Queen problem

## Problem

- Is it possible to put 8 chess queens on a 8x8 chess?
- if yes, is there other solutions? How many?
- is it possible on different board size (4x4, 10x10, 3x3...) to have a solution?

## features

- [x] Can place and remove queen manually
- [x] Pretty Command line with colors (zoom your terminal !)
- [x] Showing queen view
- [x] true chessboard coordinate !
- [X] Can solve the problem (backtracking)

![screenshot of the program](./images/sc1.png)
![screenshot of the program](./images/sc2.png)

## Compilation

Note:

- Linux terminal like (with UTF-8 support) only for now (because of the colors)
- Can work on windows (I think) if you get a Linux terminal (to support ANSI colors)
- requires boost (algorithm/string)

Using cmake :)

1. mkdir build
2. cd build
3. cmake ..
4. make
5. launch: `./build/queen_problem`
6. enjoy !
