# Laboration 5

## Compiling & Running

### make commands

`make` / `make all`

`make clean`

### Command-line args

*Default values set if no/invalid arguments are provided.*

* player name = "Player"
* First move by = Player
* Opponent type = Defender

**Without switches** (0-4 args allowed):

`lab5`

`lab5 Bob`

`lab5 Bob 0`

`lab5 Bob 1 1`

**With switches** (3 switches with corresponding args mandatory):

`-n` display name of client-player, `-s` who starts (0/1), `-t` opponent type (0/1):

`lab5 -n Bob -s 0 -t 1`

## Environment & Tools

A vagrant virtual-box running Ubuntu was used to compile and run the code using a Makefile while remaining on the
Windows host machine.

**IDE:**
Intellij Clion 2020.2.4

**OS:**
Windows 10 Ubuntu 18.04

**Compiler:**
g++ clang

**Tools:**
Vagrant 2.2.10 Virtual Box 6.1

## Purpose

A program illustrating client-server communication via a basic Tic-Tac-Toe game using a TCP socket.

## Procedures

* The client program `lab5.cpp` creates the TCP socket using the `TCPSocket` (TCPS) class which is then passed to an
  instance of the `TicTacToe` (TTT) class.
* Using a Facade design pattern, the TCPS class handles the socket connection logic and the TTT class handles all
  read/write communication with the server using the socket passed as a constructor parameter, as well as all
  game-related logic.
* Reading and writing within the TTT class is done using functions `r_read` and `r_write` in the `Restart.h` library.
* After every read or write, the array representing the game-board is updated with the latest move and printed out.
* A `while`-loop is used to continuously read/write until the server sends a message that indicates the end of the
  game (i.e. any non-MOV message).