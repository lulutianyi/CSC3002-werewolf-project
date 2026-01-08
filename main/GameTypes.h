#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace WerewolfGame {

    // 游戏角色类型
    enum class RoleType {
        VILLAGER,   // 村民
        WEREWOLF,   // 狼人
        SEER,       // 预言家
        WITCH,      // 女巫
        HUNTER      // 猎人
    };

    // 游戏阵营
    enum class Team {
        VILLAGERS,  // 村民阵营
        WEREWOLVES  // 狼人阵营
    };

    // 游戏阶段
    enum class GamePhase {
        DAY,        // 白天
        NIGHT       // 夜晚
    };

    // 玩家状态
    enum class PlayerStatus {
        ALIVE,      // 存活
        DEAD        // 死亡
    };

    // 玩家信息
    struct Player {
        int id;
        std::string name;
        RoleType role;
        Team team;
        PlayerStatus status;
        bool is_ai;

        Player(int player_id, const std::string& player_name, bool ai = false)
            : id(player_id), name(player_name), role(RoleType::VILLAGER),
            team(Team::VILLAGERS), status(PlayerStatus::ALIVE), is_ai(ai) {
        }
    };

    // 投票结果
    struct VoteResult {
        int candidate_id;
        int vote_count;
        bool is_tie;

        VoteResult() : candidate_id(-1), vote_count(0), is_tie(false) {}
    };

    // 游戏常量
    class GameConstants {
    public:
        static const int MIN_PLAYERS = 6;
        static const int MAX_PLAYERS = 12;
        static const int DEFAULT_WEREWOLF_COUNT = 2;
        static const int DEFAULT_VILLAGER_COUNT = 4;
        static const int DEFAULT_SEER_COUNT = 1;
        static const int DEFAULT_WITCH_COUNT = 1;
        static const int DEFAULT_HUNTER_COUNT = 1;
        static const int INVALID_PLAYER_ID = -1;
    };

} // namespace WerewolfGame