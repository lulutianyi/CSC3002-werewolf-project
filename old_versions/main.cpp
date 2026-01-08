#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "GameController.h"
#include "Utilities.h"

using namespace WerewolfGame;

int main() {
    std::cout << "Welcome to Werewolf Game!" << std::endl;
    std::cout << "Type 'help' for available commands." << std::endl;

    // 初始化随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    GameController game;
    bool game_initialized = false;

    std::string command;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, command);

        if (command == "quit" || command == "exit") {
            break;
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "start") {
            if (!game_initialized) {
                std::vector<std::string> player_names = {
                    "Human", "AI1", "AI2", "AI3", "AI4", "AI5", "AI6", "AI7"
                };
                game.initializeGame(8, player_names);
                game_initialized = true;
            }
            game.startGame();
        }
        else if (command == "players") {
            if (!game_initialized) {
                std::cout << "Game not initialized. Use 'start' first." << std::endl;
            }
            else {
                game.printPlayers();
            }
        }
        else if (command == "status") {
            if (!game_initialized) {
                std::cout << "Game not initialized. Use 'start' first." << std::endl;
            }
            else {
                game.printGameStatus();
            }
        }
        else if (command.find("vote") == 0) {
            if (!game_initialized) {
                std::cout << "Game not initialized. Use 'start' first." << std::endl;
            }
            else if (game.getCurrentPhase() != GamePhase::DAY) {
                std::cout << "Can only vote during day phase." << std::endl;
            }
            else {
                int voter_id, candidate_id;
                if (parseVoteCommand(command, voter_id, candidate_id)) {
                    game.processVote(voter_id, candidate_id);
                }
                else {
                    std::cout << "Invalid vote command format. Use: vote <voter_id> <candidate_id>" << std::endl;
                }
            }
        }
        else if (command == "ai_vote") {
            if (!game_initialized) {
                std::cout << "Game not initialized. Use 'start' first." << std::endl;
            }
            else if (game.getCurrentPhase() != GamePhase::DAY) {
                std::cout << "Can only vote during day phase." << std::endl;
            }
            else {
                game.processAIVotes();
            }
        }
        else if (command.empty()) {
            // Do nothing
        }
        else {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }

        // 检查游戏是否结束
        if (game_initialized && game.isGameOver()) {
            std::cout << "\nGame has ended. Use 'start' to begin a new game." << std::endl;
            game_initialized = false;
        }
    }

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}