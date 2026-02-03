#include "Board.h"
#include <sstream>
#include <cmath>
#include <algorithm>

namespace BlockBlast {

Board::Board() {
    grid_.reset();
}

bool Board::isCellOccupied(int x, int y) const {
    if (!isValidPosition(x, y)) return true;
    return grid_[toIndex(x, y)];
}

bool Board::isCellOccupied(Position pos) const {
    return isCellOccupied(pos.x, pos.y);
}

void Board::setCell(int x, int y, bool occupied) {
    if (!isValidPosition(x, y)) return;
    grid_[toIndex(x, y)] = occupied;
}

void Board::setCell(Position pos, bool occupied) {
    setCell(pos.x, pos.y, occupied);
}

bool Board::canPlacePiece(const Piece& piece, Position pos) const {
    for (const auto& cell : piece.getCells()) {
        int x = pos.x + cell.x;
        int y = pos.y + cell.y;
        
        // Check bounds and collision
        if (!isValidPosition(x, y) || isCellOccupied(x, y)) {
            return false;
        }
    }
    return true;
}

void Board::placePiece(const Piece& piece, Position pos) {
    for (const auto& cell : piece.getCells()) {
        setCell(pos.x + cell.x, pos.y + cell.y, true);
    }
}

void Board::removePiece(const Piece& piece, Position pos) {
    for (const auto& cell : piece.getCells()) {
        setCell(pos.x + cell.x, pos.y + cell.y, false);
    }
}

void Board::clear() {
    grid_.reset();
}

int Board::getOccupiedCount() const {
    return static_cast<int>(grid_.count());
}

int Board::getEmptyCount() const {
    return TOTAL_CELLS - getOccupiedCount();
}

bool Board::isEmpty() const {
    return grid_.none();
}

bool Board::isFull() const {
    return grid_.all();
}

bool Board::isRowComplete(int row) const {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        if (!grid_[toIndex(x, row)]) {
            return false;
        }
    }
    return true;
}

bool Board::isColComplete(int col) const {
    for (int y = 0; y < BOARD_SIZE; ++y) {
        if (!grid_[toIndex(col, y)]) {
            return false;
        }
    }
    return true;
}

void Board::clearRow(int row) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        grid_[toIndex(x, row)] = false;
    }
}

void Board::clearCol(int col) {
    for (int y = 0; y < BOARD_SIZE; ++y) {
        grid_[toIndex(col, y)] = false;
    }
}

Board::ClearResult Board::clearCompleteLines() {
    ClearResult result{};
    result.clearedCells.reset();
    
    // Check rows
    for (int y = 0; y < BOARD_SIZE; ++y) {
        if (isRowComplete(y)) {
            result.rowsCleared++;
            for (int x = 0; x < BOARD_SIZE; ++x) {
                result.clearedCells[toIndex(x, y)] = true;
            }
            clearRow(y);
        }
    }
    
    // Check columns
    for (int x = 0; x < BOARD_SIZE; ++x) {
        if (isColComplete(x)) {
            result.colsCleared++;
            for (int y = 0; y < BOARD_SIZE; ++y) {
                result.clearedCells[toIndex(x, y)] = true;
            }
            clearCol(x);
        }
    }
    
    result.linesCleared = result.rowsCleared + result.colsCleared;
    return result;
}

int Board::countHoles() const {
    int holes = 0;
    
    // A hole is an empty cell that has an occupied cell above it
    for (int x = 0; x < BOARD_SIZE; ++x) {
        bool foundOccupied = false;
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (isCellOccupied(x, y)) {
                foundOccupied = true;
            } else if (foundOccupied) {
                holes++;
            }
        }
    }
    
    return holes;
}

float Board::getHeightVariance() const {
    std::vector<int> heights(BOARD_SIZE, 0);
    
    // Calculate height of each column
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if (isCellOccupied(x, y)) {
                heights[x] = std::max(heights[x], BOARD_SIZE - y);
            }
        }
    }
    
    // Calculate variance
    float mean = 0.0f;
    for (int h : heights) mean += h;
    mean /= BOARD_SIZE;
    
    float variance = 0.0f;
    for (int h : heights) {
        float diff = h - mean;
        variance += diff * diff;
    }
    
    return variance / BOARD_SIZE;
}

std::vector<Position> Board::getValidPositions(const Piece& piece) const {
    std::vector<Position> positions;
    positions.reserve(TOTAL_CELLS);
    
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            Position pos(x, y);
            if (canPlacePiece(piece, pos)) {
                positions.push_back(pos);
            }
        }
    }
    
    return positions;
}

void Board::fromBitboard(uint64_t bitboard) {
    grid_ = std::bitset<TOTAL_CELLS>(bitboard);
}

std::string Board::toString() const {
    std::stringstream ss;
    
    // Top border with column numbers
    ss << "    ";
    for (int x = 0; x < BOARD_SIZE; ++x) {
        ss << x << "  ";
    }
    ss << "\n";
    
    // Top edge
    ss << "  ┌";
    for (int x = 0; x < BOARD_SIZE; ++x) {
        ss << "──";
        if (x < BOARD_SIZE - 1) ss << "─";
    }
    ss << "┐\n";
    
    // Board rows
    for (int y = 0; y < BOARD_SIZE; ++y) {
        ss << y << " │";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (isCellOccupied(x, y)) {
                ss << "██";
            } else {
                ss << "  ";
            }
            if (x < BOARD_SIZE - 1) ss << " ";
        }
        ss << "│\n";
    }
    
    // Bottom edge
    ss << "  └";
    for (int x = 0; x < BOARD_SIZE; ++x) {
        ss << "──";
        if (x < BOARD_SIZE - 1) ss << "─";
    }
    ss << "┘\n";
    
    return ss.str();
}

Board Board::copy() const {
    Board newBoard;
    newBoard.grid_ = grid_;
    return newBoard;
}

bool Board::operator==(const Board& other) const {
    return grid_ == other.grid_;
}

} // namespace BlockBlast
