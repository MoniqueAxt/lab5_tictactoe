/**
    Lab 5
    Client program implementing the server-client game TicTacToe
    @file lab5.cpp
    @author Monique Axt
    @version 1.0

    Command-line switches: (see: @file TicTacToeConstants.h)
    [-n] name of client-player (for display purposes)
    [-s] who makes the first move. 0=AI 1=client
    [-t] type of AI to play against. 0=STRIKER 1=DEFENDER

    Default values:
    player name = "Player"
    starter = PLAYER
    opponentType = DEFENDER
*/

#include <iostream>
#include <sstream>
#include <memory>

#include "TCPSocket.h"
#include "TicTacToe.h"

bool cmdLindHandler(int argc, char *argv[], std::string &name, int &starter, int &opponentType);

void setDefaults(std::string &name, int &starter, int &opponentType);

int main(int argc, char *argv[]) {
    static const int PORT = 6500;
    static const std::string IPADDRESS = "luffarserv.miun.se";
    std::string name = "Player";
    int starter = PLAYER, opponentType = DEFENDER;

    if (!cmdLindHandler(argc, argv, name, starter, opponentType)) {
        std::cout << "Missing or invalid argument/s. Using default values..." << std::endl;
        setDefaults(name, starter, opponentType);
    }

    // Connect to the server
    TCPSocket *tcpSocket;
    try {
        tcpSocket = new TCPSocket();
        tcpSocket->connect(IPADDRESS, PORT);
        std::string addr = IPADDRESS;
        std::cout << "Connected to " << addr << std::endl;

    } catch (std::runtime_error &e) {
        std::cerr << "\nError connecting to server. Exiting..." << std::endl;
        tcpSocket->close();
        return 1;
    }

    // Create a game instance
    std::unique_ptr<TicTacToe> game(new TicTacToe(tcpSocket->getDescriptor(), opponentType, starter, name));
    PrintInColor::dullGreen("Press any key to begin!");
    std::cin.get();

    if (!game->sendChallenge())
        std::cerr << "Couldn't send challenge. Exiting..." << std::endl;
    else {
        PrintInColor::dullGreen("Server accepted your challenge!");
        PrintInColor::yellow("Tic-tac-toe: " + name
                             + " vs AI" + (opponentType == STRIKER ? "-Striker" : "-Defender")
                             + "\n" + (starter == PLAYER ? "You start!" : "AI starts!"));
    }

    // Play game
    if (!game->playGame()) {
        try {
           PrintInColor::red("\nGame over! Disconnecting... ");
            tcpSocket->close();
        } catch (std::exception &e) {
            std::perror("Couldn't close socket!");
        }
    }

    return 0;
}

/*!
 * Sets the default values for the game.
 * Parameters passed by reference.
 *
 * @param name ref to variable holding the client-player name (for display purposes)
 * @param starter ref to variable holding the value indicating whether AI or client makes the first move
 * @param opponentType ref to variable holding the type of AI-player to play against
 */
void setDefaults(std::string &name, int &starter, int &opponentType) {
    name = "Player";
    starter = PLAYER;
    opponentType = DEFENDER;
}

/*!
 * Gets any command-line arguments and sets the corresponding variables.
 *
 * @param argc nr of command-line arguments
 * @param argv pointer to command-line arguments array
 * @param name ref to variable holding the client-player name (for display purposes)
 * @param starter ref to variable holding the value indicating whether AI or client makes the first move
 * @param opponentType ref to variable holding the type of AI-player to play against
 * @return true if 1+ arguments were successfully parsed, false if not (i.e. default values should be used)
 */
bool cmdLindHandler(int argc, char *argv[], std::string &name, int &starter, int &opponentType) {
    const std::string NAME_SWITCH = "-n";
    const std::string STARTER_SWITCH = "-s";
    const std::string TYPE_SWITCH = "-t";

    /* arguments without switches used */
    switch (argc) {                             // argv[0] + 0 args: use default values
        case 1:
            return false;

        case 2: {                               // argv[0] + 1 arg, no switch: player name
            std::istringstream isName(argv[1]);
            if (!(isName >> name)) {
                return false;
            }
        }
            break;

        case 3: {                               // argv[0] + 2 args, no switch: player name, who starts
            std::istringstream isName(argv[1]);
            std::istringstream isStarter(argv[2]);

            if (!(isName >> name) || !(isStarter >> starter) || (starter != PLAYER && starter != AI)) {
                return false;
            }
        }
            break;

        case 4: { // argv[0] + 3 args, no switches
            std::istringstream isName(argv[1]);         // player name
            std::istringstream isStarter(argv[2]);      // who starts
            std::istringstream isType(argv[3]);         // AI-opponent type

            // check inputs for starter and type are numerical
            if (!(isName >> name) || !(isStarter >> starter) || !(isType >> opponentType))
                return false;
            // check numerical values are in the valid range
            if ((starter != PLAYER && starter != AI) || (opponentType != STRIKER && opponentType != DEFENDER)) {
                return false;
            }
        }
            break;

            /* arguments WITH switches used */
        default: {                           // argv[0] + 6 args (+3 arguments with switches)
            for (int i = 1; i < argc; i++) {
                std::string arg = argv[i];

                // get name of player
                if (arg == NAME_SWITCH) {
                    name = argv[i + 1];
                }
                    // get argument for deciding who starts
                else if (arg == STARTER_SWITCH) {
                    std::istringstream isStarter(argv[i + 1]);
                    if (!(isStarter >> starter) || (starter != PLAYER && starter != AI)) {
                        return false;
                    }
                }
                    // get type of opponent
                else if (arg == TYPE_SWITCH) {
                    std::istringstream isType(argv[i + 1]);
                    if (!(isType >> opponentType) || (opponentType != STRIKER && opponentType != DEFENDER)) {
                        return false;
                    }
                } else {
                    std::cout << "Unknown switch " << arg << std::endl;
                    std::cout << "Usage: lab5 [-n player_name] [-s 0/1] [-t 0/1]" << std::endl;
                    std::cout << "-s " << AI << "=AI starts," << PLAYER << "=player start"
                              << "\n-t" << STRIKER << "=Striker, " << DEFENDER << "=Defender" << std::endl;
                    return false;
                }
                ++i; // Skip the value go to next switch on command line
            } // for i
        }
    }
    return true;
}



