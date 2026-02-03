#include "BotController.h"
#include "core/Piece.h"
#include <iostream>

using namespace BlockBlast;

int main() {
    std::cout << "=== Block Blast Bot - Visual Display Demo ===" << std::endl;
    
    // Create bot controller
    BotController bot;
    
    // Start with empty board
    bot.newGame();
    
    // Create some interesting pieces to visualize
    std::array<Piece, PIECES_PER_TURN> pieces = {
        PieceFactory::createPiece(Piece::Shape::L_LARGE),
        PieceFactory::createPiece(Piece::Shape::T_SHAPE),
        PieceFactory::createPiece(Piece::Shape::SQUARE_3)
    };
    
    bot.setPieces(pieces);
    
    // Display the current state
    std::cout << "\n1. INITIAL EMPTY BOARD WITH 3 PIECES" << std::endl;
    bot.displayBoard();
    bot.displayPieces();
    
    // Compute and display best moves
    std::cout << "\n2. COMPUTING OPTIMAL PLACEMENT..." << std::endl;
    auto sequence = bot.computeBestMoves();
    bot.printMoveSequence(sequence);
    
    // Execute the moves
    bot.executeSequence(sequence);
    
    std::cout << "\n3. BOARD AFTER EXECUTING MOVES" << std::endl;
    bot.displayBoard();
    
    // Add more pieces and show again
    std::cout << "\n\n=== SECOND TURN ===" << std::endl;
    pieces = {
        PieceFactory::createPiece(Piece::Shape::DOT_5),
        PieceFactory::createPiece(Piece::Shape::Z_SHAPE),
        PieceFactory::createPiece(Piece::Shape::PLUS_SHAPE)
    };
    
    bot.setPieces(pieces);
    bot.displayBoard();
    bot.displayPieces();
    
    sequence = bot.computeBestMoves();
    bot.printMoveSequence(sequence);
    
    std::cout << "\n=== Visual Display Demo Complete ===" << std::endl;
    
    return 0;
}
