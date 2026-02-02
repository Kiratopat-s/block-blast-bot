#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace BlockBlast {

// Board dimensions
constexpr int BOARD_SIZE = 8;
constexpr int TOTAL_CELLS = BOARD_SIZE * BOARD_SIZE;

// Piece constants
constexpr int MAX_PIECE_SIZE = 5;
constexpr int PIECES_PER_TURN = 3;

// Scoring weights (tunable)
struct ScoringWeights {
    float emptySpaceWeight = 1.0f;
    float comboWeight = 10.0f;
    float survivalWeight = 5.0f;
    float heightWeight = -0.5f;  // Penalize height concentration
    float holesWeight = -2.0f;   // Penalize enclosed holes
};

// Position on board
struct Position {
    int x;
    int y;
    
    constexpr Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    constexpr bool isValid() const {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }
    
    constexpr int toIndex() const {
        return y * BOARD_SIZE + x;
    }
    
    static constexpr Position fromIndex(int idx) {
        return Position(idx % BOARD_SIZE, idx / BOARD_SIZE);
    }
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

// Move representation
struct Move {
    int pieceIndex;      // Which of the 3 pieces (0-2)
    Position position;   // Where to place it
    int rotation;        // Rotation state (0-3)
    float score;         // Evaluated score
    
    Move() : pieceIndex(-1), position(), rotation(0), score(0.0f) {}
    Move(int pieceIdx, Position pos, int rot = 0) 
        : pieceIndex(pieceIdx), position(pos), rotation(rot), score(0.0f) {}
};

// Move sequence for all 3 pieces
struct MoveSequence {
    std::array<Move, PIECES_PER_TURN> moves;
    float totalScore;
    int piecesPlaced;
    
    MoveSequence() : totalScore(0.0f), piecesPlaced(0) {}
};

} // namespace BlockBlast
