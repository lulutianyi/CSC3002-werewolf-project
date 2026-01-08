#include "Utilities.h"
#include <iostream>
#include <string>

namespace WerewolfGame {

    bool parseVoteCommand(const std::string& input, int& voter_id, int& candidate_id) {
        size_t pos = input.find(' ');
        if (pos == std::string::npos || input.substr(0, pos) != "vote") {
            return false;
        }

        std::string rest = input.substr(pos + 1);
        size_t second_pos = rest.find(' ');
        if (second_pos == std::string::npos) {
            return false;
        }

        try {
            voter_id = std::stoi(rest.substr(0, second_pos));
            candidate_id = std::stoi(rest.substr(second_pos + 1));
        }
        catch (...) {
            return false;
        }

        return true;
    }

    void printHelp() {
        std::cout << "=== WEREWOLF GAME COMMANDS ===" << std::endl;
        std::cout << "help - Show this help" << std::endl;
        std::cout << "start - Start a new game" << std::endl;
        std::cout << "players - Show player list" << std::endl;
        std::cout << "status - Show game status" << std::endl;
        std::cout << "vote <voter_id> <candidate_id> - Cast a vote" << std::endl;
        std::cout << "ai_vote - Make AI players vote" << std::endl;
        std::cout << "quit - Exit the game" << std::endl;
        std::cout << "==============================" << std::endl;
    }

} // namespace WerewolfGame