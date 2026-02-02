#pragma once

#include "../../include/common.h"
#include "Piece.h"
#include <bitset>
#include <iostream>

namespace BlockBlast {

/**
 * High-performance 8x8 board using bitset for efficient operations.
 * Provides O(1) operations for most common tasks using bit manipulation.
 */
class Board {
public:
    Board();
    
    // Cell operations
    bool isCellOccupied(int x, int y) const;
    bool isCellOccupied(Position pos) const;
    void setCell(int x, int y, bool occupied);
    void setCell(Position pos, bool occupied);
    
    // Piece operations
    bool canPlacePiece(const Piece& piece, Position pos) const;
    void placePiece(const Piece& piece, Position pos);
    void removePiece(const Piece& piece, Position pos);
    
    // Board state
    void clear();
    int getOccupiedCount() const;
    int getEmptyCount() const;
    bool isEmpty() const;
    bool isFull() const;
    
    // Line clearing
    struct ClearResult {
        int linesCleared;
        int rowsCleared;
        int colsCleared;
        std::bitset<TOTAL_CELLS> clearedCells;
    };
    ClearResult clearCompleteLines();
    
    // Analysis
    int countHoles() const;  // Enclosed empty cells
    float getHeightVariance() const;
    std::vector<Position> getValidPositions(const Piece& piece) const;
    
    // Serialization
    uint64_t toBitboard() const { return grid_.to_ullong(); }
    void fromBitboard(uint64_t bitboard);
    std::string toString() const;
    
    // Copy and comparison
    Board copy() const;
    bool operator==(const Board& other) const;
    
private:
    std::bitset<TOTAL_CELLS> grid_;
    
    // Helper methods
    inline int toIndex(int x, int y) const {
        return y * BOARD_SIZE + x;
    }
    
    inline bool isValidPosition(int x, int y) const {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }
    
    bool isRowComplete(int row) const;
    bool isColComplete(int col) const;
    void clearRow(int row);
    void clearCol(int col);
};

} // namespace BlockBlast
