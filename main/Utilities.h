#pragma once
#include <string>

namespace WerewolfGame {

    // 工具函数声明
    bool parseVoteCommand(const std::string& input, int& voter_id, int& candidate_id);
    void printHelp();

} // namespace WerewolfGame