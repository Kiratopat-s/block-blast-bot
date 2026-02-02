#pragma once

#include "../../include/common.h"
#include "Board.h"
#include "Piece.h"

namespace BlockBlast {

/**
 * Encapsulates the complete game state including board, pieces, and score.
 */
class GameState {
public:
    GameState();
    explicit GameState(const std::array<Piece, PIECES_PER_TURN>& pieces);
    
    // Board access
    const Board& getBoard() const { return board_; }
    Board& getBoard() { return board_; }
    
    // Pieces access
    const std::array<Piece, PIECES_PER_TURN>& getPieces() const { return pieces_; }
    const Piece& getPiece(int index) const { return pieces_[index]; }
    bool isPieceUsed(int index) const { return piecesUsed_[index]; }
    int getRemainingPieces() const;
    
    // Move execution
    bool executeMove(const Move& move);
    void undoMove(const Move& move);
    
    // Game state
    void setBoard(const Board& board) { board_ = board; }
    void setPieces(const std::array<Piece, PIECES_PER_TURN>& pieces);
    void markPieceUsed(int index) { piecesUsed_[index] = true; }
    void markPieceUnused(int index) { piecesUsed_[index] = false; }
    
    // Score tracking
    int getScore() const { return score_; }
    void addScore(int points) { score_ += points; }
    void resetScore() { score_ = 0; }
    
    // Combo tracking
    int getComboCount() const { return comboCount_; }
    void incrementCombo() { comboCount_++; }
    void resetCombo() { comboCount_ = 0; }
    
    // Game over check
    bool isGameOver() const;
    bool hasValidMoves() const;
    
    // State management
    GameState copy() const;
    void reset();
    std::string toString() const;
    
private:
    Board board_;
    std::array<Piece, PIECES_PER_TURN> pieces_;
    std::array<bool, PIECES_PER_TURN> piecesUsed_;
    int score_;
    int comboCount_;
};

} // namespace BlockBlast
