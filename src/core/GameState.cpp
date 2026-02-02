#include "GameState.h"
#include <sstream>

namespace BlockBlast {

GameState::GameState() : score_(0), comboCount_(0) {
    piecesUsed_.fill(false);
}

GameState::GameState(const std::array<Piece, PIECES_PER_TURN>& pieces) 
    : pieces_(pieces), score_(0), comboCount_(0) {
    piecesUsed_.fill(false);
}

int GameState::getRemainingPieces() const {
    int count = 0;
    for (bool used : piecesUsed_) {
        if (!used) count++;
    }
    return count;
}

bool GameState::executeMove(const Move& move) {
    if (move.pieceIndex < 0 || move.pieceIndex >= PIECES_PER_TURN) {
        return false;
    }
    
    if (piecesUsed_[move.pieceIndex]) {
        return false;
    }
    
    const Piece& piece = pieces_[move.pieceIndex];
    
    if (!board_.canPlacePiece(piece, move.position)) {
        return false;
    }
    
    // Place the piece
    board_.placePiece(piece, move.position);
    piecesUsed_[move.pieceIndex] = true;
    
    // Clear complete lines
    auto clearResult = board_.clearCompleteLines();
    
    // Update score based on lines cleared
    if (clearResult.linesCleared > 0) {
        int lineScore = clearResult.linesCleared * 10;
        
        // Combo bonus
        comboCount_++;
        int comboBonus = comboCount_ * 5;
        
        score_ += lineScore + comboBonus;
    } else {
        comboCount_ = 0;
    }
    
    // Base score for placing piece
    score_ += piece.getSize();
    
    return true;
}

void GameState::undoMove(const Move& move) {
    if (move.pieceIndex < 0 || move.pieceIndex >= PIECES_PER_TURN) {
        return;
    }
    
    const Piece& piece = pieces_[move.pieceIndex];
    board_.removePiece(piece, move.position);
    piecesUsed_[move.pieceIndex] = false;
}

void GameState::setPieces(const std::array<Piece, PIECES_PER_TURN>& pieces) {
    pieces_ = pieces;
    piecesUsed_.fill(false);
}

bool GameState::isGameOver() const {
    return !hasValidMoves();
}

bool GameState::hasValidMoves() const {
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (piecesUsed_[i]) continue;
        
        const Piece& piece = pieces_[i];
        
        // Check all rotations
        auto rotations = piece.getAllRotations();
        for (const auto& rotatedPiece : rotations) {
            // Check all positions
            for (int y = 0; y < BOARD_SIZE; ++y) {
                for (int x = 0; x < BOARD_SIZE; ++x) {
                    if (board_.canPlacePiece(rotatedPiece, Position(x, y))) {
                        return true;
                    }
                }
            }
        }
    }
    
    return false;
}

GameState GameState::copy() const {
    GameState newState;
    newState.board_ = board_.copy();
    newState.pieces_ = pieces_;
    newState.piecesUsed_ = piecesUsed_;
    newState.score_ = score_;
    newState.comboCount_ = comboCount_;
    return newState;
}

void GameState::reset() {
    board_.clear();
    piecesUsed_.fill(false);
    score_ = 0;
    comboCount_ = 0;
}

std::string GameState::toString() const {
    std::stringstream ss;
    ss << "Score: " << score_ << " | Combo: " << comboCount_ << "\n";
    ss << "Remaining pieces: " << getRemainingPieces() << "\n";
    ss << board_.toString();
    return ss.str();
}

} // namespace BlockBlast
