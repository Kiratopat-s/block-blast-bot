#pragma once

#include "../../include/common.h"
#include "../core/GameState.h"
#include "../game/Evaluator.h"
#include <vector>
#include <memory>

namespace BlockBlast {

/**
 * High-performance AI solver using beam search and advanced pruning.
 * Finds optimal move sequences for placing 3 pieces.
 */
class Solver {
public:
    struct Config {
        int beamWidth;
        int maxDepth;
        float pruningThreshold;
        bool useParallel;
        int numThreads;              // Number of threads (0 = auto-detect)
        ScoringWeights weights;
        
        Config() 
            : beamWidth(50)
            , maxDepth(3)
            , pruningThreshold(0.3f)
            , useParallel(true)      // Enable parallel by default if OpenMP available
            , numThreads(0)          // Auto-detect thread count
            , weights() {}
    };
    
    Solver();
    explicit Solver(const Config& config);
    
    // Find best move sequence for current state
    MoveSequence findBestSequence(GameState& state);
    
    // Find best single move (greedy)
    Move findBestMove(const GameState& state, int pieceIndex);
    
    // Solve with iterative deepening
    MoveSequence solveIterativeDeepening(GameState& state, int maxTime = 5000);
    
    // Configuration
    void setConfig(const Config& config) { config_ = config; }
    const Config& getConfig() const { return config_; }
    
    // Statistics
    struct Stats {
        int nodesEvaluated = 0;
        int nodesGenerated = 0;
        int timeMs = 0;
        float bestScore = 0.0f;
    };
    
    const Stats& getStats() const { return stats_; }
    void resetStats() { stats_ = Stats(); }
    
private:
    Config config_;
    Evaluator evaluator_;
    MoveGenerator moveGenerator_;
    Stats stats_;
    
    // Beam search implementation
    struct SearchNode {
        GameState state;
        MoveSequence sequence;
        float score;
        int depth;
        
        bool operator<(const SearchNode& other) const {
            return score > other.score;  // Higher score is better
        }
    };
    
    std::vector<SearchNode> beamSearch(GameState& initialState);
    void evaluateNode(SearchNode& node);
    std::vector<SearchNode> expandNode(const SearchNode& node);
    void pruneNodes(std::vector<SearchNode>& nodes);
};

/**
 * Simple greedy solver for baseline comparison
 */
class GreedySolver {
public:
    explicit GreedySolver(const ScoringWeights& weights = ScoringWeights());
    
    MoveSequence solve(GameState& state);
    
private:
    Evaluator evaluator_;
    MoveGenerator moveGenerator_;
};

} // namespace BlockBlast
