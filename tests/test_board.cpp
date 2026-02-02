#include <cassert>
#include <iostream>
#include "../src/core/Board.h"
#include "../src/core/Piece.h"
#include "../src/core/GameState.h"

using namespace BlockBlast;

void testBoardBasics() {
    std::cout << "Testing Board basics..." << std::endl;
    
    Board board;
    assert(board.isEmpty());
    assert(board.getEmptyCount() == 64);
    assert(board.getOccupiedCount() == 0);
    
    board.setCell(0, 0, true);
    assert(!board.isEmpty());
    assert(board.getEmptyCount() == 63);
    assert(board.isCellOccupied(0, 0));
    
    board.clear();
    assert(board.isEmpty());
    
    std::cout << "✓ Board basics passed" << std::endl;
}

void testPiecePlacement() {
    std::cout << "Testing piece placement..." << std::endl;
    
    Board board;
    Piece piece = PieceFactory::createPiece(Piece::Shape::SQUARE_2);
    
    // Should fit at (0, 0)
    assert(board.canPlacePiece(piece, Position(0, 0)));
    
    board.placePiece(piece, Position(0, 0));
    assert(board.getOccupiedCount() == 4);
    
    // Should not fit at (0, 0) anymore
    assert(!board.canPlacePiece(piece, Position(0, 0)));
    
    // Should fit at (2, 0)
    assert(board.canPlacePiece(piece, Position(2, 0)));
    
    std::cout << "✓ Piece placement passed" << std::endl;
}

void testLineClearing() {
    std::cout << "Testing line clearing..." << std::endl;
    
    Board board;
    
    // Fill a complete row
    for (int x = 0; x < BOARD_SIZE; ++x) {
        board.setCell(x, 0, true);
    }
    
    auto result = board.clearCompleteLines();
    assert(result.rowsCleared == 1);
    assert(result.linesCleared == 1);
    assert(board.getOccupiedCount() == 0);
    
    // Fill a complete column
    for (int y = 0; y < BOARD_SIZE; ++y) {
        board.setCell(0, y, true);
    }
    
    result = board.clearCompleteLines();
    assert(result.colsCleared == 1);
    
    std::cout << "✓ Line clearing passed" << std::endl;
}

void testPieceRotation() {
    std::cout << "Testing piece rotation..." << std::endl;
    
    Piece lPiece = PieceFactory::createPiece(Piece::Shape::L_SMALL);
    auto rotations = lPiece.getAllRotations();
    
    // L piece should have multiple rotations
    assert(rotations.size() > 1);
    
    std::cout << "✓ Piece rotation passed" << std::endl;
}

void testGameState() {
    std::cout << "Testing game state..." << std::endl;
    
    auto pieces = PieceFactory::createRandomSet();
    GameState state(pieces);
    
    assert(state.getRemainingPieces() == 3);
    assert(state.getScore() == 0);
    
    // Try to place a piece
    Move move(0, Position(0, 0), 0);
    bool success = state.executeMove(move);
    
    if (success) {
        assert(state.getRemainingPieces() == 2);
        assert(state.isPieceUsed(0));
        assert(!state.isPieceUsed(1));
    }
    
    std::cout << "✓ Game state passed" << std::endl;
}

int main() {
    std::cout << "\n=== Running Block Blast Bot Tests ===\n" << std::endl;
    
    try {
        testBoardBasics();
        testPiecePlacement();
        testLineClearing();
        testPieceRotation();
        testGameState();
        
        std::cout << "\n✓ All tests passed!\n" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed: " << e.what() << std::endl;
        return 1;
    }
}
