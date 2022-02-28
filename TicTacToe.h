/**
    Lab 5
    Definition of the server-client game TicTacToe (Luffarschack)
    @file TicTacToe.h
    @author Monique Axt
    @version 1.0

    In order to establish a game:
     1. sendChallenge() - returns true if the server accepted the challenge
     2. playGame() - begins the game
*/

#ifndef LAB5_TCP_TICTACTOE_H
#define LAB5_TCP_TICTACTOE_H

#include <string>
#include <utility>
#include <cstring>
#include <sys/socket.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Restart.h"
#include "TicTacToeConstants.h"
#include "PrintInColor.h"
#include "StringTrim.h"

class TicTacToe {
public:
    TicTacToe(int pSocket, int pOpponentType, int pStarter, std::string pPlayerName);

    int sendChallenge();

    bool playGame();

    bool sendTestMessage(std::string testMsg) const;

private:
    int tcpSocketDesc;
    int opponentType;
    int starter;
    std::string playerName;
    char board[BOARDSIZE][BOARDSIZE]{};

    void printBoard(const char *move, int currentPlayer);

    ssize_t getReadyMessage() const;

    ssize_t sendMessageToServer(const char *msg) const;

    bool gameEnded(char *buff);

    bool gameWon(char *buff);

    const char *convertUserInput(char *buff);

    const char *convertMsgFromServer(char *buff);

};


#endif //LAB5_TCP_TICTACTOE_H
