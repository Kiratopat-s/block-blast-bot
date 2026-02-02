#pragma once

#include "../../include/common.h"
#include "../core/Board.h"
#include "../core/GameState.h"

namespace BlockBlast {

/**
 * Advanced evaluation system for scoring board positions.
 * Combines multiple heuristics for optimal move selection.
 */
class Evaluator {
public:
    explicit Evaluator(const ScoringWeights& weights = ScoringWeights());
    
    // Main evaluation function
    float evaluate(const GameState& state) const;
    float evaluate(const Board& board, const std::array<Piece, PIECES_PER_TURN>& remainingPieces) const;
    
    // Individual scoring components
    float scoreEmptySpace(const Board& board) const;
    float scoreCombo(int comboCount) const;
    float scoreSurvivalRate(const Board& board, const std::array<Piece, PIECES_PER_TURN>& pieces) const;
    float scoreHoles(const Board& board) const;
    float scoreHeightVariance(const Board& board) const;
    
    // Advanced metrics
    float calculateReachability(const Board& board) const;
    float calculateFragmentation(const Board& board) const;
    int countPotentialClears(const Board& board) const;
    
    // Configuration
    void setWeights(const ScoringWeights& weights) { weights_ = weights; }
    const ScoringWeights& getWeights() const { return weights_; }
    
private:
    ScoringWeights weights_;
    
    // Helper methods
    float normalizeScore(float score, float min, float max) const;
    int countValidPlacements(const Board& board, const Piece& piece) const;
};

/**
 * Move generator for exploring all possible placements.
 */
class MoveGenerator {
public:
    MoveGenerator() = default;
    
    // Generate all valid moves for a single piece
    std::vector<Move> generateMoves(const Board& board, const Piece& piece, int pieceIndex) const;
    
    // Generate all move sequences for all 3 pieces (considering order)
    std::vector<MoveSequence> generateAllSequences(GameState& state, int maxSequences = 10000) const;
    
    // Generate moves with pruning
    std::vector<Move> generatePrunedMoves(const Board& board, const Piece& piece, 
                                          int pieceIndex, int maxMoves = 100) const;
    
private:
    void generateMovesRecursive(GameState& state, 
                               std::vector<int> remainingIndices,
                               MoveSequence& currentSequence,
                               std::vector<MoveSequence>& allSequences,
                               int& count,
                               int maxSequences) const;
};

} // namespace BlockBlast
