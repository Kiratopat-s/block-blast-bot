#include "Piece.h"
#include <algorithm>
#include <random>
#include <sstream>

namespace BlockBlast {

Piece::Piece() : shape_(Shape::CUSTOM), width_(0), height_(0) {}

Piece::Piece(Shape shape) : shape_(shape) {
    cells_ = getShapeCells(shape);
    normalize();
    calculateBounds();
}

Piece::Piece(const std::vector<Position>& cells) 
    : shape_(Shape::CUSTOM), cells_(cells) {
    normalize();
    calculateBounds();
}

void Piece::calculateBounds() {
    if (cells_.empty()) {
        width_ = height_ = 0;
        return;
    }
    
    int minX = cells_[0].x, maxX = cells_[0].x;
    int minY = cells_[0].y, maxY = cells_[0].y;
    
    for (const auto& cell : cells_) {
        minX = std::min(minX, cell.x);
        maxX = std::max(maxX, cell.x);
        minY = std::min(minY, cell.y);
        maxY = std::max(maxY, cell.y);
    }
    
    width_ = maxX - minX + 1;
    height_ = maxY - minY + 1;
}

void Piece::normalize() {
    if (cells_.empty()) return;
    
    int minX = cells_[0].x;
    int minY = cells_[0].y;
    
    for (const auto& cell : cells_) {
        minX = std::min(minX, cell.x);
        minY = std::min(minY, cell.y);
    }
    
    for (auto& cell : cells_) {
        cell.x -= minX;
        cell.y -= minY;
    }
}

Piece Piece::rotate90() const {
    std::vector<Position> rotated;
    rotated.reserve(cells_.size());
    
    for (const auto& cell : cells_) {
        // Rotate 90° clockwise: (x, y) -> (y, -x)
        rotated.push_back(Position(cell.y, -cell.x));
    }
    
    return Piece(rotated);
}

Piece Piece::rotate180() const {
    std::vector<Position> rotated;
    rotated.reserve(cells_.size());
    
    for (const auto& cell : cells_) {
        // Rotate 180°: (x, y) -> (-x, -y)
        rotated.push_back(Position(-cell.x, -cell.y));
    }
    
    return Piece(rotated);
}

Piece Piece::rotate270() const {
    std::vector<Position> rotated;
    rotated.reserve(cells_.size());
    
    for (const auto& cell : cells_) {
        // Rotate 270° clockwise: (x, y) -> (-y, x)
        rotated.push_back(Position(-cell.y, cell.x));
    }
    
    return Piece(rotated);
}

std::vector<Piece> Piece::getAllRotations() const {
    std::vector<Piece> rotations;
    rotations.reserve(4);
    
    rotations.push_back(*this);
    auto r90 = rotate90();
    auto r180 = rotate180();
    auto r270 = rotate270();
    
    // Only add unique rotations
    if (r90.getCells() != cells_) rotations.push_back(r90);
    if (r180.getCells() != cells_ && r180.getCells() != r90.getCells()) {
        rotations.push_back(r180);
    }
    if (r270.getCells() != cells_ && r270.getCells() != r90.getCells() && 
        r270.getCells() != r180.getCells()) {
        rotations.push_back(r270);
    }
    
    return rotations;
}

std::string Piece::toString() const {
    if (cells_.empty()) return "Empty piece";
    
    std::stringstream ss;
    
    // Top border
    ss << "  ┌";
    for (int x = 0; x < width_; ++x) {
        ss << "──";
        if (x < width_ - 1) ss << "─";
    }
    ss << "┐\n";
    
    // Piece rows
    for (int y = 0; y < height_; ++y) {
        ss << "  │";
        for (int x = 0; x < width_; ++x) {
            bool found = false;
            for (const auto& cell : cells_) {
                if (cell.x == x && cell.y == y) {
                    found = true;
                    break;
                }
            }
            if (found) {
                ss << "██";
            } else {
                ss << "  ";
            }
            if (x < width_ - 1) ss << " ";
        }
        ss << "│\n";
    }
    
    // Bottom border
    ss << "  └";
    for (int x = 0; x < width_; ++x) {
        ss << "──";
        if (x < width_ - 1) ss << "─";
    }
    ss << "┘ (" << width_ << "x" << height_ << ")\n";
    
    return ss.str();
}

std::vector<Position> Piece::getShapeCells(Shape shape) {
    switch (shape) {
        case Shape::SINGLE:
            return {{0, 0}};
            
        case Shape::DOT_2:
            return {{0, 0}, {1, 0}};
            
        case Shape::DOT_3:
            return {{0, 0}, {1, 0}, {2, 0}};
            
        case Shape::DOT_4:
            return {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
            
        case Shape::DOT_5:
            return {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}};
            
        case Shape::SQUARE_2:
            return {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
            
        case Shape::SQUARE_3:
            return {{0, 0}, {1, 0}, {2, 0},
                    {0, 1}, {1, 1}, {2, 1},
                    {0, 2}, {1, 2}, {2, 2}};
            
        case Shape::L_SMALL:
            return {{0, 0}, {0, 1}, {1, 1}};
            
        case Shape::L_LARGE:
            return {{0, 0}, {0, 1}, {0, 2}, {1, 2}};
            
        case Shape::T_SHAPE:
            return {{0, 0}, {1, 0}, {2, 0}, {1, 1}};
            
        case Shape::Z_SHAPE:
            return {{0, 0}, {1, 0}, {1, 1}, {2, 1}};
            
        case Shape::PLUS_SHAPE:
            return {{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};
            
        case Shape::CORNER_3:
            return {{0, 0}, {1, 0}, {0, 1}};
            
        default:
            return {};
    }
}

// PieceFactory implementations
Piece PieceFactory::createPiece(Piece::Shape shape) {
    return Piece(shape);
}

std::vector<Piece> PieceFactory::getAllPieceShapes() {
    return {
        Piece(Piece::Shape::SINGLE),
        Piece(Piece::Shape::DOT_2),
        Piece(Piece::Shape::DOT_3),
        Piece(Piece::Shape::DOT_4),
        Piece(Piece::Shape::DOT_5),
        Piece(Piece::Shape::SQUARE_2),
        Piece(Piece::Shape::SQUARE_3),
        Piece(Piece::Shape::L_SMALL),
        Piece(Piece::Shape::L_LARGE),
        Piece(Piece::Shape::T_SHAPE),
        Piece(Piece::Shape::Z_SHAPE),
        Piece(Piece::Shape::PLUS_SHAPE),
        Piece(Piece::Shape::CORNER_3)
    };
}

Piece PieceFactory::createRandomPiece() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 12);
    
    return createPiece(static_cast<Piece::Shape>(dis(gen)));
}

std::array<Piece, PIECES_PER_TURN> PieceFactory::createRandomSet() {
    std::array<Piece, PIECES_PER_TURN> pieces;
    for (auto& piece : pieces) {
        piece = createRandomPiece();
    }
    return pieces;
}

} // namespace BlockBlast
