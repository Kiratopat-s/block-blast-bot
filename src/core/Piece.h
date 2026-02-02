#pragma once

#include "../../include/common.h"
#include <bitset>

namespace BlockBlast {

/**
 * Represents a game piece with efficient bitboard representation.
 * Uses bitset for fast collision detection and placement validation.
 */
class Piece {
public:
    // Piece shapes (similar to Block Blast game pieces)
    enum class Shape {
        SINGLE,      // 1x1
        DOT_2,       // 2x1
        DOT_3,       // 3x1
        DOT_4,       // 4x1
        DOT_5,       // 5x1
        SQUARE_2,    // 2x2
        SQUARE_3,    // 3x3
        L_SMALL,     // Small L
        L_LARGE,     // Large L
        T_SHAPE,     // T shape
        Z_SHAPE,     // Z shape
        PLUS_SHAPE,  // + shape
        CORNER_3,    // 3x3 corner
        // Add more shapes as needed
        CUSTOM
    };
    
    Piece();
    explicit Piece(Shape shape);
    Piece(const std::vector<Position>& cells);
    
    // Getters
    Shape getShape() const { return shape_; }
    const std::vector<Position>& getCells() const { return cells_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    int getSize() const { return static_cast<int>(cells_.size()); }
    
    // Rotation (returns new piece)
    Piece rotate90() const;
    Piece rotate180() const;
    Piece rotate270() const;
    
    // Get all rotations (cache for performance)
    std::vector<Piece> getAllRotations() const;
    
    // Utility
    bool isEmpty() const { return cells_.empty(); }
    std::string toString() const;
    
private:
    Shape shape_;
    std::vector<Position> cells_;  // Relative positions
    int width_;
    int height_;
    
    void calculateBounds();
    void normalize();  // Move piece to (0,0) origin
    
    // Static piece definitions
    static std::vector<Position> getShapeCells(Shape shape);
};

/**
 * Factory for creating predefined pieces
 */
class PieceFactory {
public:
    static Piece createPiece(Piece::Shape shape);
    static std::vector<Piece> getAllPieceShapes();
    static Piece createRandomPiece();
    static std::array<Piece, PIECES_PER_TURN> createRandomSet();
};

} // namespace BlockBlast
