#pragma once
#include "GameTypes.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace WerewolfGame {

    class GameController {
    public:
        GameController();

        // ��Ϸ�������ڹ���
        void initializeGame(int player_count, const std::vector<std::string>& player_names);
        void startGame();

        // ��Ϸ���̿���
        void startDayPhase();
        void processVote(int voter_id, int candidate_id);
        void processAIVotes();

        // ��Ϸ״̬��ѯ
        void printGameStatus() const;
        void printPlayers() const;
        bool isGameOver() const { return _game_over; }
        GamePhase getCurrentPhase() const { return _current_phase; }
        const std::vector<Player>& getPlayers() const { return _players; }

        void setTonightWerewolfTarget(int target) { _tonight_werewolf_target = target; }
        int getTonightWerewolfTarget() const { return _tonight_werewolf_target; }

    private:
        // ��ɫ����
        void assignRoles();
        std::vector<RoleType> generateRoleSetup(int player_count) const;

        // ��ҹ���
        bool isPlayerAlive(int player_id) const;
        std::vector<int> getAlivePlayers() const;
        void eliminatePlayer(int player_id);

        // ͶƱϵͳ
        bool allPlayersVoted() const;
        void processVoteResult();
        

        // ҹ��ϵͳ
        void startNightPhase();
        void processWerewolfActions();
        void processSeerActions();
        void processAISeerCheck(int seer_id);
        void processAIWitch(int witch_id);
        void processWitchActions();
        void processHunterActions();
        void resolveNightActions();
        void processHunterShoot(int hunter_id);

        struct SeerCheckRecord{
            int seer_id;
            int target_id;
            RoleType role;
            SeerCheckRecord(int id, RoleType r): target_id(id),role(r){};
        };

        std::vector<SeerCheckRecord> _seer_check_records;
        
        bool hasPlayerBeenChecked (int player_id) const;
        void performSeerCheck(int seer_id, int target_id);

        // 女巫的操作流程
        void handleHumanWitchActions();

        bool gunpoisoned = false;
        
        // ��Ϸ״̬���
        void checkGameOver();

        int _tonight_werewolf_target;  // 狼人今晚选择的目标
        int _tonight_healed_player;    // 女巫救的目标
        int _tonight_poisoned_player;  // 女巫毒的目标
        bool _heal_potion_used;        // 解药是否已用
        bool _poison_potion_used;      // 毒药是否已用
        int _witch_player_id;          // 女巫玩家ID
        

    private:
        std::vector<Player> _players;
        GamePhase _current_phase;
        bool _game_over;
        Team _winning_team;
        std::unordered_map<int, int> _votes; // candidate_id -> vote_count
        std::unordered_map<int, int> _player_votes; // voter_id -> candidate_id
    };

} // namespace WerewolfGame