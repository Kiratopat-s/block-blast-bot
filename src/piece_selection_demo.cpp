#include "BotController.h"
#include <iostream>

using namespace BlockBlast;

int main() {
    std::cout << "=== Visual Piece Selection Demo ===" << std::endl;
    std::cout << "This demonstrates the new interactive piece selection interface." << std::endl;
    std::cout << std::endl;
    
    BotController bot;
    
    // Show just the gallery
    std::cout << "1. PIECE GALLERY VIEW" << std::endl;
    std::cout << "   This is what users see when selecting pieces:" << std::endl;
    std::cout << std::endl;
    
    // Call the private method via a workaround - create a simple display
    std::cout << "======================================================================" << std::endl;
    std::cout << "  PIECE GALLERY - All Available Shapes" << std::endl;
    std::cout << "======================================================================" << std::endl;
    
    const std::vector<std::pair<Piece::Shape, std::string>> pieces = {
        {Piece::Shape::SINGLE, "Single"},
        {Piece::Shape::DOT_2, "Dot 2"},
        {Piece::Shape::DOT_3, "Dot 3"},
        {Piece::Shape::DOT_4, "Dot 4"},
        {Piece::Shape::DOT_5, "Dot 5"},
        {Piece::Shape::SQUARE_2, "Square 2x2"},
        {Piece::Shape::SQUARE_3, "Square 3x3"},
        {Piece::Shape::L_SMALL, "L Small"},
        {Piece::Shape::L_LARGE, "L Large"},
        {Piece::Shape::T_SHAPE, "T Shape"},
        {Piece::Shape::Z_SHAPE, "Z Shape"},
        {Piece::Shape::PLUS_SHAPE, "Plus"},
        {Piece::Shape::CORNER_3, "Corner 3x3"}
    };
    
    for (size_t i = 0; i < pieces.size(); ++i) {
        auto piece = PieceFactory::createPiece(pieces[i].first);
        std::cout << "\n[" << (i + 1) << "] " << pieces[i].second << " (" 
                  << piece.getWidth() << "x" << piece.getHeight() << ")" << std::endl;
        std::cout << piece.toString();
    }
    
    std::cout << "\n======================================================================" << std::endl;
    
    // Show rotation example
    std::cout << "\n\n2. ROTATION PATTERN VIEWER" << std::endl;
    std::cout << "   Example: L Large piece with all rotations:" << std::endl;
    std::cout << std::endl;
    
    auto lPiece = PieceFactory::createPiece(Piece::Shape::L_LARGE);
    auto rotations = lPiece.getAllRotations();
    
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "  L LARGE - Rotation Patterns (" << rotations.size() << " unique)" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    
    for (size_t i = 0; i < rotations.size(); ++i) {
        std::cout << "\nRotation " << i << " (" << (i * 90) << "°):" << std::endl;
        std::cout << rotations[i].toString();
    }
    
    std::cout << "----------------------------------------------------------------------" << std::endl;
    
    // Show T shape rotations
    std::cout << "\n\n3. ANOTHER EXAMPLE: T Shape Rotations" << std::endl;
    std::cout << std::endl;
    
    auto tPiece = PieceFactory::createPiece(Piece::Shape::T_SHAPE);
    auto tRotations = tPiece.getAllRotations();
    
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "  T SHAPE - Rotation Patterns (" << tRotations.size() << " unique)" << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    
    for (size_t i = 0; i < tRotations.size(); ++i) {
        std::cout << "\nRotation " << i << " (" << (i * 90) << "°):" << std::endl;
        std::cout << tRotations[i].toString();
    }
    
    std::cout << "----------------------------------------------------------------------" << std::endl;
    
    std::cout << "\n\n=== Interactive Selection Process ===" << std::endl;
    std::cout << "When you run ./block-blast-bot in interactive mode:" << std::endl;
    std::cout << "1. Choose 'Use empty board'" << std::endl;
    std::cout << "2. See the visual gallery of all pieces" << std::endl;
    std::cout << "3. Type 'R' to view rotations of any piece" << std::endl;
    std::cout << "4. Type 1-13 to select a specific piece" << std::endl;
    std::cout << "5. Type 0 for a random piece" << std::endl;
    std::cout << "6. Repeat for 3 pieces total" << std::endl;
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}
