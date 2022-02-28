/**
    Lab 5
    Implementation of the server-client game TicTacToe (Luffarschack)
    @file TicTacToe.cpp
    @author Monique Axt
    @version 1.0
*/

#include "TicTacToe.h"

/* ******************************************************
  PUBLIC methods
 ********************************************************/
/*!
 * Constructor that initialises all member variables
 *
 * @param pSocket open socket that is connected to the server
 * @param pOpponentType the type of AI-opponent on the server to challenge
 * @param pStarter which player makes the first move (1=AI, 0=client)
 * @param pPlayerName name of the client player
 * */
TicTacToe::TicTacToe(const int pSocket, const int pOpponentType, const int pStarter, std::string pPlayerName)
        : tcpSocketDesc(pSocket), opponentType(pOpponentType), starter(pStarter), playerName(std::move(pPlayerName)) {

    // initialise the game-board
    for (auto &i : board) {       //y
        for (char &j : i) {   //x
            j = '-';
        }
    }
}

/*!
 * Adds a game-move to the array representing the game-board and prints the contents
 * to standard output.
 *
 * @param move
 * @param currentPlayer
 */
void TicTacToe::printBoard(const char *move, int currentPlayer) {
    std::string moveStr = move;
    std::replace(moveStr.begin(), moveStr.end(), ',', ' ');

    std::stringstream ss(moveStr);
    int x, y;

    if (!(ss >> x >> y)) {
        std::clog << "Couldn't print board: invalid input values";
        return;
    }

    int n = 0;
    // print horizontal row of numbers
    while (n < 15)
        std::cout << "   " << std::left << std::setw(2) << n++;

    std::cout << "\n";
    for (int i = 0; i < BOARDSIZE; i++) {
        // print vertical row of numbers
        std::cout << std::left << std::setw(3) << i;

        // print board chars
        for (int j = 0; j < BOARDSIZE; j++) {
            if (i == x && j == y) {
                board[i][j] = (currentPlayer == AI ? '0' : 'X');
            }
            std::cout << std::left << std::setw(2) << board[i][j] << "   ";

            if (j == 14)
                std::cout << "\n";
        }
    }
    std::cout << std::endl;
}

/*!
 * Handles writing-to and reading-from the server.
 * Until the game is won or illegal input is sent, this function continuously retreives and sends commands
 * to the server, converting the messages to the appropriate format.
 *
 * @return false when the game ends
 */
bool TicTacToe::playGame() {
    char buff[100];
    int n = 0;
    bzero(buff, sizeof(buff));

    if (starter == PLAYER) {
        printf("->%s: ", playerName.c_str());
        while ((buff[n++] = getchar()) != '\n')
            continue;
        r_write(tcpSocketDesc, (void *) convertUserInput(buff), sizeof(buff));
        printBoard(buff, PLAYER);
    }

    while (true) {
        bzero(buff, sizeof(buff));
        n = 0;

        // READ FROM SERVER
        r_read(tcpSocketDesc, buff, sizeof(buff));
        printf("<-Server: %s", convertMsgFromServer(buff));

        if (gameEnded(buff) || gameWon(buff))   // break out of while-loop and end game
            break;

        printBoard(convertMsgFromServer(buff), AI);
        bzero(buff, sizeof(buff));

        // WRITE TO SERVER
        printf("->%s: ", playerName.c_str());
        while ((buff[n++] = getchar()) != '\n')
            continue;

        auto convertedInput = convertUserInput(buff);
        r_write(tcpSocketDesc, (void *) convertedInput, sizeof(buff));

        if (std::string(convertedInput) == "QUI") {
            return false;                       // end the game if client sends quit-message
        }
        printBoard(buff, PLAYER);
    }
    return false;
}

/*!
 * Initiates a game with the server by sending a CHA (challenge) message, reads the
 * OKR (ok ready) message from the server and prints both to output.
 *
 * @return true if the CHA message was successfully sent and the OKR message was successfully read.
 */
int TicTacToe::sendChallenge() {
    std::string nameLen;
    if (playerName.length() < 10)
        nameLen = "0" + std::to_string(playerName.length());

    std::string challengeMsg = CHALLENGE
                               + std::to_string(opponentType) + DELIM
                               + std::to_string(starter) + DELIM
                               + nameLen + DELIM
                               + playerName + "\n";

    auto byteswritten = sendMessageToServer(challengeMsg.c_str());
    auto bytesread = getReadyMessage();

    return (byteswritten && bytesread);
}

/*!
 * Used to send a test message to the server in order to verify reading and writing to the server works.
 *
 * @param testMsg
 * @return
 */
bool TicTacToe::sendTestMessage(std::string testMsg) const {
    testMsg += "\n";
    auto msg = (testMsg).c_str();

    auto byteswritten = r_write(tcpSocketDesc, (void *) msg, sizeof(msg));
    if (byteswritten > 0)
        std::cout << "Success - test message sent\n";
    else std::cerr << "Error - couldn't send test message." << std::endl;

    char buffer[20];
    auto bytesread = r_read(tcpSocketDesc, buffer, sizeof(buffer));
    printf("Wrote: %ld Read: %ld\n%s\n", byteswritten, bytesread, buffer);

    if (bytesread > 0 && byteswritten > 0)
        return true;

    return false;
}


/* ******************************************************
  PRIVATE methods
 ********************************************************/
/*!
 * Wrapper function used to read the OKR (ok ready) message from the server after
 * sending a CHA (challenge) message.
 *
 * @return true if the read was successful
 */
ssize_t TicTacToe::getReadyMessage() const {
    char buffer[4];
    return r_read(tcpSocketDesc, buffer, sizeof(buffer));
}

/*!
 * Wrapper function to send a message to the server.
 *
 * @param msg
 * @return
 */
ssize_t TicTacToe::sendMessageToServer(const char *msg) const {
    return r_write(tcpSocketDesc, (void *) msg, strlen(msg));
}

/*!
 * Used to determine if the server has ended the game.
 * Any non-move message (i.e. does not start with "MOV") ends the game.
 *
 * @param buff buffer containing the message from the server
 * @return true if the buffer contains a non-move message
 */
bool TicTacToe::gameEnded(char *buff) {
    return !strstr(buff, "MOV");
}

/*!
 * Used to determine if the server sent a message indicating the game has been won.
 *
 * @param buff buffer containing the message from the server
 * @return true if the server-message contains "WIN"
 */
bool TicTacToe::gameWon(char *buff) {
    return strstr(buff, "WIN") != nullptr;
}

/*!
 * Converts the user/client input from [X(X),Y(Y)] to the format [MOV:XX:YY]
 * Out-of-range exception caught for substr() function
 *
 * @param buff buffer containing the user input
 * @return the converted input
 */
const char *TicTacToe::convertUserInput(char *buff) {
    if (strstr(buff, "QUI")) {
        return "QUI";
    }

    std::string moveStr = buff;
    StringTrim::trim(moveStr);
    std::replace(moveStr.begin(), moveStr.end(), ',', ':');

    // if leading zeros need to be added to the numbers
    if (moveStr.length() <= 4) {  // 00:00
        try {
            std::string firstNum = moveStr.substr(0, moveStr.find(':'));
            std::string secondNum = moveStr.substr(firstNum.length() + 1);
            moveStr = (firstNum.length() == 1 ? "0" : "") + firstNum + DELIM
                      + (secondNum.length() == 1 ? "0" : "") + secondNum;
        } catch (std::out_of_range &e) {
            std::cerr << "Invalid input. Only numbers are accepted in the format:[xx,yy]." << std::endl;
        }
    }
    std::stringstream ss;
    ss << MOVE << moveStr << "\n";
    moveStr = ss.str();
    const char *moveMessage = moveStr.c_str();
    return moveMessage;
}

/*!
 * Converts MOV (move) messages from the server from [MOV:XX:YY] to [X(X),Y(Y)].
 *
 * @param buff buffer containing the message
 * @return the converted message if MOV-message, or the unchanged buffer param if it's a non-MOV message.
 */
const char *TicTacToe::convertMsgFromServer(char *buff) {
    if (gameEnded(buff)) {
        return buff;
    }

    if (gameWon(buff)) {
        return buff;
    }

    std::string moveStr = buff;
    try {
        StringTrim::trim(moveStr);
        moveStr.erase(0, 4);
        std::replace(moveStr.begin(), moveStr.end(), ':', ',');

        // remove leading zeros
        if (moveStr[0] == '0')
            moveStr.erase(0, 1);

        if (moveStr[moveStr.find(',') + 1] == '0')
            moveStr.erase(moveStr.find(',') + 1, 1);

    } catch (std::exception &e) {
        std::clog << "Couldn't convert msg from server: " << e.what();
    }
    const char *moveMessage = (moveStr += "\n").c_str();
    return moveMessage;
}











