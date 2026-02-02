#include "Evaluator.h"
#include <algorithm>
#include <cmath>
#include <limits>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace BlockBlast {

Evaluator::Evaluator(const ScoringWeights& weights) : weights_(weights) {}

float Evaluator::evaluate(const GameState& state) const {
    const Board& board = state.getBoard();
    
    float score = 0.0f;
    
    // Empty space score (more empty is better)
    score += weights_.emptySpaceWeight * scoreEmptySpace(board);
    
    // Combo score
    score += weights_.comboWeight * scoreCombo(state.getComboCount());
    
    // Survival rate (can we place remaining pieces?)
    std::array<Piece, PIECES_PER_TURN> remainingPieces;
    int idx = 0;
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (!state.isPieceUsed(i)) {
            remainingPieces[idx++] = state.getPiece(i);
        }
    }
    score += weights_.survivalWeight * scoreSurvivalRate(board, remainingPieces);
    
    // Penalize holes and height variance
    score += weights_.holesWeight * scoreHoles(board);
    score += weights_.heightWeight * scoreHeightVariance(board);
    
    return score;
}

float Evaluator::evaluate(const Board& board, const std::array<Piece, PIECES_PER_TURN>& remainingPieces) const {
    float score = 0.0f;
    
    score += weights_.emptySpaceWeight * scoreEmptySpace(board);
    score += weights_.survivalWeight * scoreSurvivalRate(board, remainingPieces);
    score += weights_.holesWeight * scoreHoles(board);
    score += weights_.heightWeight * scoreHeightVariance(board);
    
    return score;
}

float Evaluator::scoreEmptySpace(const Board& board) const {
    // More empty cells = better
    return static_cast<float>(board.getEmptyCount());
}

float Evaluator::scoreCombo(int comboCount) const {
    // Exponential bonus for combos
    if (comboCount <= 0) return 0.0f;
    return std::pow(2.0f, static_cast<float>(comboCount));
}

float Evaluator::scoreSurvivalRate(const Board& board, const std::array<Piece, PIECES_PER_TURN>& pieces) const {
    int totalValidPlacements = 0;
    int totalPieces = 0;
    
    #ifdef _OPENMP
    // Parallel counting for multiple pieces
    #pragma omp parallel for reduction(+:totalValidPlacements,totalPieces) if(PIECES_PER_TURN >= 2)
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        const auto& piece = pieces[i];
        if (!piece.isEmpty()) {
            totalPieces++;
            int validPlacements = countValidPlacements(board, piece);
            totalValidPlacements += validPlacements;
        }
    }
    #else
    // Serial version
    for (const auto& piece : pieces) {
        if (piece.isEmpty()) continue;
        
        totalPieces++;
        int validPlacements = countValidPlacements(board, piece);
        totalValidPlacements += validPlacements;
    }
    #endif
    
    if (totalPieces == 0) return 0.0f;
    
    // Return average number of valid placements per piece
    return static_cast<float>(totalValidPlacements) / totalPieces;
}

float Evaluator::scoreHoles(const Board& board) const {
    // Holes are bad - return negative count
    return static_cast<float>(board.countHoles());
}

float Evaluator::scoreHeightVariance(const Board& board) const {
    // High variance is bad (uneven surface)
    return board.getHeightVariance();
}

float Evaluator::calculateReachability(const Board& board) const {
    // Calculate how many cells are reachable (not isolated)
    int reachableCells = 0;
    
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (!board.isCellOccupied(x, y)) {
                // Check if at least one neighbor is also empty
                bool hasEmptyNeighbor = false;
                const int dx[] = {-1, 1, 0, 0};
                const int dy[] = {0, 0, -1, 1};
                
                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                        if (!board.isCellOccupied(nx, ny)) {
                            hasEmptyNeighbor = true;
                            break;
                        }
                    }
                }
                
                if (hasEmptyNeighbor) {
                    reachableCells++;
                }
            }
        }
    }
    
    return static_cast<float>(reachableCells);
}

float Evaluator::calculateFragmentation(const Board& board) const {
    // Count number of separated empty regions (lower is better)
    // Simple approximation: count empty cells with no empty neighbors
    int isolatedCells = 0;
    
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (!board.isCellOccupied(x, y)) {
                int emptyNeighbors = 0;
                const int dx[] = {-1, 1, 0, 0};
                const int dy[] = {0, 0, -1, 1};
                
                for (int i = 0; i < 4; ++i) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                        if (!board.isCellOccupied(nx, ny)) {
                            emptyNeighbors++;
                        }
                    }
                }
                
                if (emptyNeighbors == 0) {
                    isolatedCells++;
                }
            }
        }
    }
    
    return static_cast<float>(isolatedCells);
}

int Evaluator::countPotentialClears(const Board& board) const {
    int potentialClears = 0;
    
    // Count almost-complete rows
    for (int y = 0; y < BOARD_SIZE; ++y) {
        int occupiedCount = 0;
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (board.isCellOccupied(x, y)) {
                occupiedCount++;
            }
        }
        if (occupiedCount >= BOARD_SIZE - 2) {
            potentialClears++;
        }
    }
    
    // Count almost-complete columns
    for (int x = 0; x < BOARD_SIZE; ++x) {
        int occupiedCount = 0;
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (board.isCellOccupied(x, y)) {
                occupiedCount++;
            }
        }
        if (occupiedCount >= BOARD_SIZE - 2) {
            potentialClears++;
        }
    }
    
    return potentialClears;
}

float Evaluator::normalizeScore(float score, float min, float max) const {
    if (max - min < 0.001f) return 0.0f;
    return (score - min) / (max - min);
}

int Evaluator::countValidPlacements(const Board& board, const Piece& piece) const {
    int count = 0;
    
    // Check all rotations
    auto rotations = piece.getAllRotations();
    
    #ifdef _OPENMP
    // Parallel counting across rotations and positions
    #pragma omp parallel for reduction(+:count) collapse(2) if(rotations.size() > 1)
    for (size_t r = 0; r < rotations.size(); ++r) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (board.canPlacePiece(rotations[r], Position(x, y))) {
                    count++;
                }
            }
        }
    }
    #else
    // Serial version
    for (const auto& rotatedPiece : rotations) {
        // Check all positions
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (board.canPlacePiece(rotatedPiece, Position(x, y))) {
                    count++;
                }
            }
        }
    }
    #endif
    
    return count;
}

// MoveGenerator implementations
std::vector<Move> MoveGenerator::generateMoves(const Board& board, const Piece& piece, int pieceIndex) const {
    std::vector<Move> moves;
    moves.reserve(TOTAL_CELLS * 4);  // Estimate: all positions * rotations
    
    // Get all rotations
    auto rotations = piece.getAllRotations();
    
    for (size_t rotIdx = 0; rotIdx < rotations.size(); ++rotIdx) {
        const auto& rotatedPiece = rotations[rotIdx];
        
        // Try all positions
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                Position pos(x, y);
                if (board.canPlacePiece(rotatedPiece, pos)) {
                    Move move(pieceIndex, pos, static_cast<int>(rotIdx));
                    moves.push_back(move);
                }
            }
        }
    }
    
    return moves;
}

std::vector<MoveSequence> MoveGenerator::generateAllSequences(GameState& state, int maxSequences) const {
    std::vector<MoveSequence> allSequences;
    allSequences.reserve(maxSequences);
    
    std::vector<int> remainingIndices;
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (!state.isPieceUsed(i)) {
            remainingIndices.push_back(i);
        }
    }
    
    MoveSequence currentSequence;
    int count = 0;
    
    generateMovesRecursive(state, remainingIndices, currentSequence, allSequences, count, maxSequences);
    
    return allSequences;
}

void MoveGenerator::generateMovesRecursive(GameState& state, 
                                          std::vector<int> remainingIndices,
                                          MoveSequence& currentSequence,
                                          std::vector<MoveSequence>& allSequences,
                                          int& count,
                                          int maxSequences) const {
    if (count >= maxSequences) return;
    
    if (remainingIndices.empty() || currentSequence.piecesPlaced >= PIECES_PER_TURN) {
        if (currentSequence.piecesPlaced > 0) {
            allSequences.push_back(currentSequence);
            count++;
        }
        return;
    }
    
    // Try each remaining piece
    for (size_t i = 0; i < remainingIndices.size(); ++i) {
        int pieceIdx = remainingIndices[i];
        const Piece& piece = state.getPiece(pieceIdx);
        
        // Generate all moves for this piece
        auto moves = generateMoves(state.getBoard(), piece, pieceIdx);
        
        for (const auto& move : moves) {
            // Apply move
            if (state.executeMove(move)) {
                // Add to sequence
                currentSequence.moves[currentSequence.piecesPlaced] = move;
                currentSequence.piecesPlaced++;
                
                // Create new remaining indices
                std::vector<int> newRemaining;
                for (size_t j = 0; j < remainingIndices.size(); ++j) {
                    if (j != i) {
                        newRemaining.push_back(remainingIndices[j]);
                    }
                }
                
                // Recurse
                generateMovesRecursive(state, newRemaining, currentSequence, allSequences, count, maxSequences);
                
                // Undo move
                state.undoMove(move);
                currentSequence.piecesPlaced--;
            }
        }
    }
    
    // Also consider stopping here (not placing all pieces)
    if (currentSequence.piecesPlaced > 0) {
        allSequences.push_back(currentSequence);
        count++;
    }
}

std::vector<Move> MoveGenerator::generatePrunedMoves(const Board& board, const Piece& piece, 
                                                     int pieceIndex, int maxMoves) const {
    auto allMoves = generateMoves(board, piece, pieceIndex);
    
    // If within limit, return all
    if (allMoves.size() <= static_cast<size_t>(maxMoves)) {
        return allMoves;
    }
    
    // Simple pruning: sample evenly across the board
    std::vector<Move> pruned;
    pruned.reserve(maxMoves);
    
    int step = allMoves.size() / maxMoves;
    for (size_t i = 0; i < allMoves.size(); i += step) {
        if (pruned.size() >= static_cast<size_t>(maxMoves)) break;
        pruned.push_back(allMoves[i]);
    }
    
    return pruned;
}

} // namespace BlockBlast
