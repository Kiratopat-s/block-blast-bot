#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <set>
#include <algorithm>
#include <iomanip>

// Forward declaration - ClearedBoardInfo will be defined after Board class

// ============================================================
//                    PRINT UTILITIES NAMESPACE
// ============================================================
namespace PrintUtils {
    // ANSI Color codes
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string GRAY    = "\033[90m";
    
    // Cell characters
    const char FILLED_CELL = '#';
    const char EMPTY_CELL  = ' ';
}

class Coordinate {
    private:
        int x;
        int y;
    public:
        Coordinate(int xVal, int yVal) : x(xVal), y(yVal) {}
        int getX() const { return x; }
        int getY() const { return y; }
};

class Box {
    private:
        std::vector<Coordinate> body;
        std::string name;
    public:
        Box(const std::string& boxName, const std::vector<Coordinate>& boxBody)
            : name(boxName), body(boxBody) {}
        
        std::string getName() const { return name; }
        const std::vector<Coordinate>& getBody() const { return body; }
};

class SquareBox : public Box {
    public:
        SquareBox(int size) : Box("SquareBox", createBody(size)) {}
        // in current version have 1x1, 2x2, 3x3 sizes

    private:
        static std::vector<Coordinate> createBody(int size) {
            std::vector<Coordinate> body;
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    body.push_back(Coordinate(i, j));
                }
            }
            return body;
        }

};


class RectangleBox : public Box {
    public:
        RectangleBox(int width, int height) : Box("RectangleBox", createBody(width, height)) {}
        // in current version have LINE 1x2, 2x1, 1x3, 3x1, 1x4, 4x1, 1x5, 5x1 
        // and BLOCK 2x3, 3x2 sizes

    private:
        static std::vector<Coordinate> createBody(int width, int height) {
            std::vector<Coordinate> body;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    body.push_back(Coordinate(i, j));
                }
            }
            return body;
        }
};

class LShapeBox : public Box {
    public:
        LShapeBox(std::string side, int rotation) : Box("LShapeBox", createBody(side, rotation)) {}
        // in current version have L shape of size 3

    private:
        static std::vector<Coordinate> createBody(std::string side, int rotation) {
            std::vector<Coordinate> body;
            switch (rotation) {
                case 0:
                    if (side == "left") {
                        body.push_back(Coordinate(0, 0));
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(2, 0));
                        body.push_back(Coordinate(2, 1));
                    } else if (side == "right") {
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(2, 1));
                        body.push_back(Coordinate(2, 0));
                    }
                    break;
                case 90:
                    if (side == "left") {
                        body.push_back(Coordinate(0, 0));
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(0, 2));
                        body.push_back(Coordinate(1, 0));
                    } else if (side == "right") {
                        body.push_back(Coordinate(0, 2));
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(0, 0));
                        body.push_back(Coordinate(1, 2));
                    }
                    break;
                case 180:
                    if (side == "left") {
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(2, 1));
                        body.push_back(Coordinate(0, 0));
                    } else if (side == "right") {
                        body.push_back(Coordinate(0, 0));
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(2, 0));
                        body.push_back(Coordinate(0, 1));
                    }
                    break;
                case 270:
                    if (side == "left") {
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(1, 2));
                        body.push_back(Coordinate(0, 2));
                    } else if (side == "right") {
                        body.push_back(Coordinate(1, 2));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(0, 0));
                    }
                    break;
                default:
                    break;
            }
            return body;
        }
};

class ConnerBox : public Box {
    public:
        ConnerBox(int size, int rotation) : Box("ConnerBox", createBody(size, rotation)) {}

    private:
        static std::vector<Coordinate> createBody(int size, int rotation) {
            std::vector<Coordinate> body;
            switch (rotation) {
                case 0:
                    for (int i = 0; i < size; ++i) {
                        body.push_back(Coordinate(0, i));
                        body.push_back(Coordinate(i, 0));
                    }
                    break;
                case 90:
                    for (int i = 0; i < size; ++i) {
                        body.push_back(Coordinate(i, size - 1));
                        body.push_back(Coordinate(0, i));
                    }
                    break;
                case 180:
                    for (int i = 0; i < size; ++i) {
                        body.push_back(Coordinate(size - 1, i));
                        body.push_back(Coordinate(i, size - 1));
                    }
                    break;
                case 270:
                    for (int i = 0; i < size; ++i) {
                        body.push_back(Coordinate(i, 0));
                        body.push_back(Coordinate(size - 1, i));
                    }
                    break;
                default:
                    break;
            }
            return body;
        }
};

class TShapeBox : public Box {
    public:
        TShapeBox(int rotation) : Box("TShapeBox", createBody(rotation)) {}
    private:
        static std::vector<Coordinate> createBody(int rotation) {
            std::vector<Coordinate> body;
            switch (rotation) {
                case 0:
                    body.push_back(Coordinate(0, 0));
                    body.push_back(Coordinate(0, 1));
                    body.push_back(Coordinate(0, 2));
                    body.push_back(Coordinate(1, 1));
                    break;
                case 90:
                    body.push_back(Coordinate(0, 1));
                    body.push_back(Coordinate(1, 1));
                    body.push_back(Coordinate(2, 1));
                    body.push_back(Coordinate(1, 0));
                    break;
                case 180:
                    body.push_back(Coordinate(1, 0));
                    body.push_back(Coordinate(1, 1));
                    body.push_back(Coordinate(1, 2));
                    body.push_back(Coordinate(0, 1));
                    break;
                case 270:
                    body.push_back(Coordinate(0, 0));
                    body.push_back(Coordinate(1, 0));
                    body.push_back(Coordinate(2, 0));
                    body.push_back(Coordinate(1, 1));
                    break;
                default:
                    break;
            }
            return body;
        }
};

class ZShapeBox : public Box {
    public:
        ZShapeBox(std::string side, int rotation) : Box("ZShapeBox", createBody(side, rotation)) {}
    private:
        static std::vector<Coordinate> createBody(std::string side, int rotation) {
            std::vector<Coordinate> body;
            if (side == "left") {
                switch (rotation) {
                    case 0:
                    case 180:
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(0, 2));
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(1, 1));
                        break;
                    case 90:
                    case 270:
                        body.push_back(Coordinate(0, 0));
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(2, 1));
                        break;
                    default:
                        break;
                }
            }
            else if (side == "right") {
                switch (rotation) {
                    case 0:
                    case 180:
                        body.push_back(Coordinate(0, 0));
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(1, 2));
                        break;
                    case 90:
                    case 270:
                        body.push_back(Coordinate(0, 1));
                        body.push_back(Coordinate(1, 1));
                        body.push_back(Coordinate(1, 0));
                        body.push_back(Coordinate(2, 0));
                        break;
                    default:
                        break;
                }
            }
            return body;
        }
};


class DiagonalBox : public Box {
    public:
        DiagonalBox(std::string side, int size) : Box("DiagonalBox", createBody(side, size)) {}
    private:
        static std::vector<Coordinate> createBody(std::string side, int size) {
            std::vector<Coordinate> body;
            if (side == "left") {
                for (int i = 0; i < size; ++i) {
                    body.push_back(Coordinate(i, i));
                }
            } else if (side == "right") {
                for (int i = 0; i < size; ++i) {
                    body.push_back(Coordinate(i, size - 1 - i));
                }
            }
            return body;
        }
};

std::vector<Box> BoxBundle() {
    std::vector<Box> boxes;
    boxes.push_back(SquareBox(1));
    boxes.push_back(SquareBox(2));
    boxes.push_back(SquareBox(3));
    boxes.push_back(RectangleBox(1, 2));
    boxes.push_back(RectangleBox(2, 1));
    boxes.push_back(RectangleBox(1, 3));
    boxes.push_back(RectangleBox(3, 1));
    boxes.push_back(RectangleBox(1, 4));
    boxes.push_back(RectangleBox(4, 1));
    boxes.push_back(RectangleBox(1, 5));
    boxes.push_back(RectangleBox(5, 1));
    boxes.push_back(RectangleBox(2, 3));
    boxes.push_back(RectangleBox(3, 2));
    boxes.push_back(LShapeBox("left", 0));
    boxes.push_back(LShapeBox("left", 90));
    boxes.push_back(LShapeBox("left", 180));
    boxes.push_back(LShapeBox("left", 270));
    boxes.push_back(LShapeBox("right", 0));
    boxes.push_back(LShapeBox("right", 90));
    boxes.push_back(LShapeBox("right", 180));
    boxes.push_back(LShapeBox("right", 270));
    boxes.push_back(ConnerBox(2, 0));
    boxes.push_back(ConnerBox(2, 90));
    boxes.push_back(ConnerBox(2, 180));
    boxes.push_back(ConnerBox(2, 270));
    boxes.push_back(ConnerBox(3, 0));
    boxes.push_back(ConnerBox(3, 90));
    boxes.push_back(ConnerBox(3, 180));
    boxes.push_back(ConnerBox(3, 270));
    boxes.push_back(TShapeBox(0));
    boxes.push_back(TShapeBox(90));
    boxes.push_back(TShapeBox(180));
    boxes.push_back(TShapeBox(270));
    boxes.push_back(ZShapeBox("left", 0));
    boxes.push_back(ZShapeBox("left", 90));
    boxes.push_back(ZShapeBox("right", 0));
    boxes.push_back(ZShapeBox("right", 90));
    boxes.push_back(DiagonalBox("left", 2));
    boxes.push_back(DiagonalBox("right", 2));
    boxes.push_back(DiagonalBox("left", 3));
    boxes.push_back(DiagonalBox("right", 3));
    return boxes;
}

// ============================================================
//                    PRINT UTILITY FUNCTIONS
// ============================================================

// Print a box visual on a small preview board
void printBoxVisualOnBoard(const Box& box, int startX, int startY, int boardSize) {
    std::vector<std::vector<char>> board(boardSize, std::vector<char>(boardSize, PrintUtils::EMPTY_CELL));
    for (const auto& coord : box.getBody()) {
        int x = startX + coord.getX();
        int y = startY + coord.getY();
        if (x < boardSize && y < boardSize) {
            board[x][y] = PrintUtils::FILLED_CELL;
        }
    }
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            std::cout << "[" << board[i][j] << "]";
        }
        std::cout << std::endl;
    }
}

// Print a box visual with custom color
void printBoxVisualWithColor(const Box& box, int startX, int startY, int boardSize, const std::string& color) {
    std::vector<std::vector<char>> board(boardSize, std::vector<char>(boardSize, PrintUtils::EMPTY_CELL));
    for (const auto& coord : box.getBody()) {
        int x = startX + coord.getX();
        int y = startY + coord.getY();
        if (x < boardSize && y < boardSize) {
            board[x][y] = PrintUtils::FILLED_CELL;
        }
    }
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j] == PrintUtils::FILLED_CELL) {
                std::cout << "[" << color << board[i][j] << PrintUtils::RESET << "]";
            } else {
                std::cout << "[" << board[i][j] << "]";
            }
        }
        std::cout << std::endl;
    }
}

void printMenuNumber(int number, bool highlighted = false) {
    if (highlighted) {
        std::cout << PrintUtils::MAGENTA << "  " << number << "." << PrintUtils::RESET;
    } else {
        std::cout << PrintUtils::YELLOW << "  " << number << "." << PrintUtils::RESET;
    }
}

Box MenuBoxSelection() {
    system("clear");
    int mainChoice;
    int subChoice;
    
    while (true) {
        std::cout << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
        std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                       📦 BOX SELECTION MENU 📦                        " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
        std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
        std::cout << "Please select a box type from the following options:\n\n";
        printMenuNumber(1);
        std::cout << " SquareBox      (1x1, 2x2, 3x3)\n";
        printMenuNumber(2);
        std::cout << " RectangleBox   (Lines and blocks)\n";
        printMenuNumber(3);
        std::cout << " LShapeBox      (L-shaped pieces)\n";
        printMenuNumber(4);
        std::cout << " ConnerBox      (Corner pieces)\n";
        printMenuNumber(5);
        std::cout << " TShapeBox      (T-shaped pieces)\n";
        printMenuNumber(6);
        std::cout << " ZShapeBox      (Z-shaped pieces)\n";
        printMenuNumber(7);
        std::cout << " DiagonalBox    (Diagonal pieces)\n";
        std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
        std::cout << "Enter your choice (1-7): ";
        std::cin >> mainChoice;
        system("clear");
        
        switch (mainChoice) {
            case 1: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                     ■ SQUARE BOX SELECTION ■                         " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select size for SquareBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " Size 1x1" << std::endl;
                    printBoxVisualWithColor(SquareBox(1), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " Size 2x2" << std::endl;
                    printBoxVisualWithColor(SquareBox(2), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " Size 3x3" << std::endl;
                    printBoxVisualWithColor(SquareBox(3), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-3): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    if (subChoice >= 1 && subChoice <= 3) {
                        return SquareBox(subChoice);
                    }
                    std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    system("clear");
                }
                break;
            }
            case 2: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                   ▬ RECTANGLE BOX SELECTION ▬                        " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select type for RectangleBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " LINE 1x2" << std::endl;
                    printBoxVisualWithColor(RectangleBox(1, 2), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " LINE 2x1" << std::endl;
                    printBoxVisualWithColor(RectangleBox(2, 1), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " LINE 1x3" << std::endl;
                    printBoxVisualWithColor(RectangleBox(1, 3), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(4);
                    std::cout << " LINE 3x1" << std::endl;
                    printBoxVisualWithColor(RectangleBox(3, 1), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(5);
                    std::cout << " LINE 1x4" << std::endl;
                    printBoxVisualWithColor(RectangleBox(1, 4), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(6);
                    std::cout << " LINE 4x1" << std::endl;
                    printBoxVisualWithColor(RectangleBox(4, 1), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(7);
                    std::cout << " LINE 1x5" << std::endl;
                    printBoxVisualWithColor(RectangleBox(1, 5), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(8);
                    std::cout << " LINE 5x1" << std::endl;
                    printBoxVisualWithColor(RectangleBox(5, 1), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(9);
                    std::cout << " BLOCK 2x3" << std::endl;
                    printBoxVisualWithColor(RectangleBox(2, 3), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(10);
                    std::cout << " BLOCK 3x2" << std::endl;
                    printBoxVisualWithColor(RectangleBox(3, 2), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-10): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    switch (subChoice) {
                        case 1: return RectangleBox(1, 2);
                        case 2: return RectangleBox(2, 1);
                        case 3: return RectangleBox(1, 3);
                        case 4: return RectangleBox(3, 1);
                        case 5: return RectangleBox(1, 4);
                        case 6: return RectangleBox(4, 1);
                        case 7: return RectangleBox(1, 5);
                        case 8: return RectangleBox(5, 1);
                        case 9: return RectangleBox(2, 3);
                        case 10: return RectangleBox(3, 2);
                        default:
                            std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                            std::cout << "Press Enter to continue...";
                            std::cin.ignore();
                            std::cin.get();
                            system("clear");
                    }
                }
                break;
            }
            case 3: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                     ⌐ L-SHAPE BOX SELECTION ⌐                        " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select type for LShapeBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " Left L, Rotation 0" << std::endl;
                    printBoxVisualWithColor(LShapeBox("left", 0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " Left L, Rotation 90" << std::endl;
                    printBoxVisualWithColor(LShapeBox("left", 90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " Left L, Rotation 180" << std::endl;
                    printBoxVisualWithColor(LShapeBox("left", 180), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(4);
                    std::cout << " Left L, Rotation 270" << std::endl;
                    printBoxVisualWithColor(LShapeBox("left", 270), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(5);
                    std::cout << " Right L, Rotation 0" << std::endl;
                    printBoxVisualWithColor(LShapeBox("right", 0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(6);
                    std::cout << " Right L, Rotation 90" << std::endl;
                    printBoxVisualWithColor(LShapeBox("right", 90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(7);
                    std::cout << " Right L, Rotation 180" << std::endl;
                    printBoxVisualWithColor(LShapeBox("right", 180), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(8);
                    std::cout << " Right L, Rotation 270" << std::endl;
                    printBoxVisualWithColor(LShapeBox("right", 270), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-8): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    switch (subChoice) {
                        case 1: return LShapeBox("left", 0);
                        case 2: return LShapeBox("left", 90);
                        case 3: return LShapeBox("left", 180);
                        case 4: return LShapeBox("left", 270);
                        case 5: return LShapeBox("right", 0);
                        case 6: return LShapeBox("right", 90);
                        case 7: return LShapeBox("right", 180);
                        case 8: return LShapeBox("right", 270);
                        default:
                            std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                            std::cout << "Press Enter to continue...";
                            std::cin.ignore();
                            std::cin.get();
                            system("clear");
                    }
                }
                break;
            }
            case 4: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                     ┌ CORNER BOX SELECTION ┌                         " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select type for ConnerBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " Size 2, Rotation 0" << std::endl;
                    printBoxVisualWithColor(ConnerBox(2, 0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " Size 2, Rotation 90" << std::endl;
                    printBoxVisualWithColor(ConnerBox(2, 90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " Size 2, Rotation 180" << std::endl;
                    printBoxVisualWithColor(ConnerBox(2, 180), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(4);
                    std::cout << " Size 2, Rotation 270" << std::endl;
                    printBoxVisualWithColor(ConnerBox(2, 270), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(5);
                    std::cout << " Size 3, Rotation 0" << std::endl;
                    printBoxVisualWithColor(ConnerBox(3, 0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(6);
                    std::cout << " Size 3, Rotation 90" << std::endl;
                    printBoxVisualWithColor(ConnerBox(3, 90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(7);
                    std::cout << " Size 3, Rotation 180" << std::endl;
                    printBoxVisualWithColor(ConnerBox(3, 180), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(8);
                    std::cout << " Size 3, Rotation 270" << std::endl;
                    printBoxVisualWithColor(ConnerBox(3, 270), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-8): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    switch (subChoice) {
                        case 1: return ConnerBox(2, 0);
                        case 2: return ConnerBox(2, 90);
                        case 3: return ConnerBox(2, 180);
                        case 4: return ConnerBox(2, 270);
                        case 5: return ConnerBox(3, 0);
                        case 6: return ConnerBox(3, 90);
                        case 7: return ConnerBox(3, 180);
                        case 8: return ConnerBox(3, 270);
                        default:
                            std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                            std::cout << "Press Enter to continue...";
                            std::cin.ignore();
                            std::cin.get();
                            system("clear");
                    }
                }
                break;
            }
            case 5: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                     ┬ T-SHAPE BOX SELECTION ┬                        " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select rotation for TShapeBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " Rotation 0" << std::endl;
                    printBoxVisualWithColor(TShapeBox(0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " Rotation 90" << std::endl;
                    printBoxVisualWithColor(TShapeBox(90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " Rotation 180" << std::endl;
                    printBoxVisualWithColor(TShapeBox(180), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(4);
                    std::cout << " Rotation 270" << std::endl;
                    printBoxVisualWithColor(TShapeBox(270), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-4): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    switch (subChoice) {
                        case 1: return TShapeBox(0);
                        case 2: return TShapeBox(90);
                        case 3: return TShapeBox(180);
                        case 4: return TShapeBox(270);
                        default:
                            std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                            std::cout << "Press Enter to continue...";
                            std::cin.ignore();
                            std::cin.get();
                            system("clear");
                    }
                }
                break;
            }
            case 6: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                     ⌐ Z-SHAPE BOX SELECTION ⌐                        " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select type for ZShapeBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " Left Z, Horizontal (0/180)" << std::endl;
                    printBoxVisualWithColor(ZShapeBox("left", 0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " Left Z, Vertical (90/270)" << std::endl;
                    printBoxVisualWithColor(ZShapeBox("left", 90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " Right Z, Horizontal (0/180)" << std::endl;
                    printBoxVisualWithColor(ZShapeBox("right", 0), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(4);
                    std::cout << " Right Z, Vertical (90/270)" << std::endl;
                    printBoxVisualWithColor(ZShapeBox("right", 90), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-4): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    switch (subChoice) {
                        case 1: return ZShapeBox("left", 0);
                        case 2: return ZShapeBox("left", 90);
                        case 3: return ZShapeBox("right", 0);
                        case 4: return ZShapeBox("right", 90);
                        default:
                            std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                            std::cout << "Press Enter to continue...";
                            std::cin.ignore();
                            std::cin.get();
                            system("clear");
                    }
                }
                break;
            }
            case 7: {
                while (true) {
                    std::cout << PrintUtils::GREEN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "║" << PrintUtils::RESET << "                     ⟋ DIAGONAL BOX SELECTION ⟋                        " << PrintUtils::GREEN << "║" << PrintUtils::RESET << "\n";
                    std::cout << PrintUtils::GREEN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                    std::cout << "Select type for DiagonalBox:\n";
                    std::cout << PrintUtils::RED << "\n  0. ← Go Back to Main Menu" << PrintUtils::RESET << "\n";
                    std::cout << "\n";
                    printMenuNumber(1);
                    std::cout << " Left Diagonal, Size 2" << std::endl;
                    printBoxVisualWithColor(DiagonalBox("left", 2), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(2);
                    std::cout << " Right Diagonal, Size 2" << std::endl;
                    printBoxVisualWithColor(DiagonalBox("right", 2), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(3);
                    std::cout << " Left Diagonal, Size 3" << std::endl;
                    printBoxVisualWithColor(DiagonalBox("left", 3), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n";
                    printMenuNumber(4);
                    std::cout << " Right Diagonal, Size 3" << std::endl;
                    printBoxVisualWithColor(DiagonalBox("right", 3), 0, 0, 5, PrintUtils::CYAN);
                    std::cout << "\n" << PrintUtils::GRAY << "─────────────────────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
                    std::cout << "Enter your choice (0-4): ";
                    std::cin >> subChoice;
                    if (subChoice == 0) {
                        system("clear");
                        break; // Go back to main menu
                    }
                    switch (subChoice) {
                        case 1: return DiagonalBox("left", 2);
                        case 2: return DiagonalBox("right", 2);
                        case 3: return DiagonalBox("left", 3);
                        case 4: return DiagonalBox("right", 3);
                        default:
                            std::cout << PrintUtils::RED << "Invalid choice. Please try again." << PrintUtils::RESET << std::endl;
                            std::cout << "Press Enter to continue...";
                            std::cin.ignore();
                            std::cin.get();
                            system("clear");
                    }
                }
                break;
            }
            default:
                std::cout << PrintUtils::RED << "Invalid choice. Please enter a number between 1 and 7." << PrintUtils::RESET << std::endl;
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                system("clear");
                break;
        }
    }
}

void NotifySelectedBox(Box selectedBox) {
    system("clear");
    std::cout << "You have selected the box: " << selectedBox.getName() << std::endl;
    printBoxVisualOnBoard(selectedBox, 0, 0, 5);
}

class Board {
    private:
        static const int BOARD_SIZE = 8;
        static const char FILLED_CELL = '#';
        static const char EMPTY_CELL = ' ';
        bool board[BOARD_SIZE][BOARD_SIZE];

    public:
        Board() {
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    board[row][col] = false;
                }
            }
        }

        int getBoardSize() const { return BOARD_SIZE; }

        bool isCellEmpty(int x, int y) const {
            if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
                return false;
            }
            return !board[x][y];
        }

        bool isCellFilled(int x, int y) const {
            if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
                return false;
            }
            return board[x][y];
        }

        bool isInBounds(int x, int y) const {
            return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
        }

        void placeBlock(int x, int y) {
            if (isInBounds(x, y)) {
                board[x][y] = true;
            }
        }

        void removeBlock(int x, int y) {
            if (isInBounds(x, y)) {
                board[x][y] = false;
            }
        }

        void randomlyPlaceBlocks() {
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    board[row][col] = (rand() % 2 == 1);
                }
            }
        }

        void printColumnHeaders() const {
            std::cout << "   ";
            for (int col = 0; col < BOARD_SIZE; ++col) {
                std::cout << " " << col << " ";
            }
            std::cout << std::endl;
        }

        void printNumeric() const {
            printColumnHeaders();
            for (int row = 0; row < BOARD_SIZE; ++row) {
                std::cout << " " << row << " ";
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    std::cout << "[" << board[row][col] << "]";
                }
                std::cout << std::endl;
            }
        }

        void printVisual() const {
            printColumnHeaders();
            for (int row = 0; row < BOARD_SIZE; ++row) {
                std::cout << " " << row << " ";
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    char cellChar = board[row][col] ? FILLED_CELL : EMPTY_CELL;
                    std::cout << "[" << cellChar << "]";
                }
                std::cout << std::endl;
            }
        }

        void display(bool visualMode = true) const {
            if (visualMode) {
                printVisual();
            } else {
                printNumeric();
            }
        }

        void printEmptyCells() const {
            std::cout << "Empty Cells (row, col):" << std::endl;
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    if (!board[row][col]) {
                        std::cout << "(" << row << ", " << col << ")" << std::endl;
                    }
                }
            }
        }

        int getEmptyCellCount() const {
            int count = 0;
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    if (!board[row][col]) {
                        ++count;
                    }
                }
            }
            return count;
        }
};

// ============================================================
//           BOARD PRINT UTILITY FUNCTIONS (requires Board)
// ============================================================

// Print a Board with specific cells highlighted in a given color
void printBoardWithHighlight(const Board& boardToPrint, 
                             const std::vector<std::pair<int, int>>& highlightCoords,
                             const std::string& highlightColor = PrintUtils::RED) {
    // Print column headers
    std::cout << "   ";
    for (int col = 0; col < boardToPrint.getBoardSize(); ++col) {
        std::cout << " " << col << " ";
    }
    std::cout << std::endl;
    
    // Print board with highlights
    for (int row = 0; row < boardToPrint.getBoardSize(); ++row) {
        std::cout << " " << row << " ";
        for (int col = 0; col < boardToPrint.getBoardSize(); ++col) {
            // Check if this coordinate should be highlighted
            bool isHighlighted = false;
            for (const auto& coord : highlightCoords) {
                if (coord.first == row && coord.second == col) {
                    isHighlighted = true;
                    break;
                }
            }
            
            if (isHighlighted) {
                // Highlighted color for specified cells
                std::cout << "[" << highlightColor << PrintUtils::FILLED_CELL << PrintUtils::RESET << "]";
            } else if (boardToPrint.isCellFilled(row, col)) {
                // Normal color for existing filled cells
                std::cout << "[" << PrintUtils::FILLED_CELL << "]";
            } else {
                // Empty cell
                std::cout << "[" << PrintUtils::EMPTY_CELL << "]";
            }
        }
        std::cout << std::endl;
    }
}

// Print a Board with two different highlight colors (e.g., placed cells vs cleared cells)
void printBoardWithDualHighlight(const Board& boardToPrint,
                                  const std::vector<std::pair<int, int>>& primaryCoords,
                                  const std::vector<std::pair<int, int>>& secondaryCoords,
                                  const std::string& primaryColor = PrintUtils::RED,
                                  const std::string& secondaryColor = PrintUtils::GREEN) {
    // Print column headers
    std::cout << "   ";
    for (int col = 0; col < boardToPrint.getBoardSize(); ++col) {
        std::cout << " " << col << " ";
    }
    std::cout << std::endl;
    
    // Print board with highlights
    for (int row = 0; row < boardToPrint.getBoardSize(); ++row) {
        std::cout << " " << row << " ";
        for (int col = 0; col < boardToPrint.getBoardSize(); ++col) {
            bool isPrimary = false;
            bool isSecondary = false;
            
            for (const auto& coord : primaryCoords) {
                if (coord.first == row && coord.second == col) {
                    isPrimary = true;
                    break;
                }
            }
            for (const auto& coord : secondaryCoords) {
                if (coord.first == row && coord.second == col) {
                    isSecondary = true;
                    break;
                }
            }
            
            if (isPrimary) {
                std::cout << "[" << primaryColor << PrintUtils::FILLED_CELL << PrintUtils::RESET << "]";
            } else if (isSecondary) {
                std::cout << "[" << secondaryColor << PrintUtils::FILLED_CELL << PrintUtils::RESET << "]";
            } else if (boardToPrint.isCellFilled(row, col)) {
                std::cout << "[" << PrintUtils::FILLED_CELL << "]";
            } else {
                std::cout << "[" << PrintUtils::EMPTY_CELL << "]";
            }
        }
        std::cout << std::endl;
    }
}

// Print a Board with multiple piece placements in different colors
// pieceCoordsList[0] = first piece coords (RED), [1] = second (GREEN), [2] = third (YELLOW), etc.
void printBoardWithMultiPieceHighlight(const Board& boardToPrint,
                                        const std::vector<std::vector<std::pair<int, int>>>& pieceCoordsList,
                                        const Board& originalBoard) {
    // Color palette for different pieces: RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN
    std::vector<std::string> pieceColors = {
        PrintUtils::RED, PrintUtils::GREEN, PrintUtils::YELLOW,
        PrintUtils::BLUE, PrintUtils::MAGENTA, PrintUtils::CYAN
    };
    
    // Print column headers
    std::cout << "   ";
    for (int col = 0; col < boardToPrint.getBoardSize(); ++col) {
        std::cout << " " << col << " ";
    }
    std::cout << std::endl;
    
    // Print board with multi-color highlights
    for (int row = 0; row < boardToPrint.getBoardSize(); ++row) {
        std::cout << " " << row << " ";
        for (int col = 0; col < boardToPrint.getBoardSize(); ++col) {
            // Check which piece (if any) this coordinate belongs to
            int pieceIndex = -1;
            for (size_t p = 0; p < pieceCoordsList.size(); ++p) {
                for (const auto& coord : pieceCoordsList[p]) {
                    if (coord.first == row && coord.second == col) {
                        pieceIndex = static_cast<int>(p);
                        break;
                    }
                }
                if (pieceIndex >= 0) break;
            }
            
            if (pieceIndex >= 0) {
                // This cell belongs to a placed piece - use piece color
                std::string color = pieceColors[pieceIndex % pieceColors.size()];
                std::cout << "[" << color << PrintUtils::FILLED_CELL << PrintUtils::RESET << "]";
            } else if (originalBoard.isCellFilled(row, col)) {
                // This cell was in the original board - default color
                std::cout << "[" << PrintUtils::FILLED_CELL << "]";
            } else if (boardToPrint.isCellFilled(row, col)) {
                // Filled but not tracked (shouldn't happen normally)
                std::cout << "[" << PrintUtils::FILLED_CELL << "]";
            } else {
                // Empty cell
                std::cout << "[" << PrintUtils::EMPTY_CELL << "]";
            }
        }
        std::cout << std::endl;
    }
    
    // Print legend
    std::cout << PrintUtils::GRAY << "   Legend: ";
    for (size_t i = 0; i < std::min(pieceCoordsList.size(), pieceColors.size()); ++i) {
        std::cout << pieceColors[i] << "[#]" << PrintUtils::RESET << "=Piece " << (i+1) << " ";
    }
    std::cout << "[#]=Original" << PrintUtils::RESET << "\n";
}

// Struct to hold information about a single placement step
struct PlacementStep {
    int pieceIndex;                              // Which piece (index in original list)
    std::pair<int, int> position;                // Where it was placed (row, col)
    std::vector<std::pair<int, int>> pieceCoords; // Coordinates of piece cells
    int clearedRows;                             // Rows cleared after this placement
    int clearedCols;                             // Cols cleared after this placement
    int cellsCleared;                            // Total cells cleared after this placement
    
    PlacementStep() : pieceIndex(0), position({0, 0}), clearedRows(0), clearedCols(0), cellsCleared(0) {}
};

// Struct to hold a complete placement sequence (all 3 pieces with blasting)
struct PlacementSequence {
    Board finalBoard;                            // Board state after all placements and blasts
    std::vector<PlacementStep> steps;            // Each placement step
    std::vector<int> pieceOrder;                 // Order of pieces placed (permutation)
    int totalCellsCleared;                       // Total cells cleared across all blasts
    int totalLinesCleared;                       // Total rows + cols cleared
    int score;                                   // Final calculated score
    
    PlacementSequence() : totalCellsCleared(0), totalLinesCleared(0), score(0) {}
};

// Struct to hold board scoring information for strategy evaluation
struct BoardScore {
    Board board;
    int score;
    int emptyCells;
    int potentialRowClears;      // Rows with 7+ filled cells
    int potentialColClears;      // Columns with 7+ filled cells
    int almostFullRows;          // Rows with 6+ filled cells
    int almostFullCols;          // Columns with 6+ filled cells
    int centerControl;           // Cells filled in center 4x4 area
    int edgeSpread;              // How spread out pieces are on edges
    int totalCellsCleared;       // NEW: Total cells cleared during placement sequence
    int totalLinesCleared;       // NEW: Total lines cleared during placement sequence
    size_t boardIndex;           // Original index for reference
    std::vector<std::vector<std::pair<int, int>>> pieceCoords;  // Coords for each piece placement
    std::vector<int> pieceOrder;                                 // NEW: Order pieces were placed
    std::vector<std::pair<int, int>> piecePositions;            // NEW: Positions for each piece
    
    BoardScore() : score(0), emptyCells(0), potentialRowClears(0), potentialColClears(0),
                   almostFullRows(0), almostFullCols(0), centerControl(0), edgeSpread(0),
                   totalCellsCleared(0), totalLinesCleared(0), boardIndex(0) {}
};

// Function to calculate board score based on multiple strategic factors
BoardScore calculateBoardScore(const Board& b, size_t index) {
    BoardScore bs;
    bs.board = b;
    bs.boardIndex = index;
    
    const int boardSize = b.getBoardSize();
    bs.emptyCells = b.getEmptyCellCount();
    
    // Check each row for potential clears
    for (int row = 0; row < boardSize; ++row) {
        int filledCount = 0;
        for (int col = 0; col < boardSize; ++col) {
            if (b.isCellFilled(row, col)) filledCount++;
        }
        if (filledCount == boardSize) bs.potentialRowClears++;      // Full row (will clear)
        else if (filledCount >= 7) bs.potentialRowClears++;         // Almost full
        else if (filledCount >= 6) bs.almostFullRows++;
    }
    
    // Check each column for potential clears
    for (int col = 0; col < boardSize; ++col) {
        int filledCount = 0;
        for (int row = 0; row < boardSize; ++row) {
            if (b.isCellFilled(row, col)) filledCount++;
        }
        if (filledCount == boardSize) bs.potentialColClears++;      // Full column (will clear)
        else if (filledCount >= 7) bs.potentialColClears++;         // Almost full
        else if (filledCount >= 6) bs.almostFullCols++;
    }
    
    // Calculate center control (center 4x4 area)
    for (int row = 2; row < 6; ++row) {
        for (int col = 2; col < 6; ++col) {
            if (b.isCellFilled(row, col)) bs.centerControl++;
        }
    }
    
    // Calculate edge spread (corners and edges)
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            if (b.isCellFilled(row, col)) {
                if (row == 0 || row == boardSize-1 || col == 0 || col == boardSize-1) {
                    bs.edgeSpread++;
                }
            }
        }
    }
    
    // Calculate final score (higher is better)
    // Priority: Clearing lines > Almost clearing > Empty cells > Center control
    bs.score = (bs.potentialRowClears * 100) +    // High priority for potential clears
               (bs.potentialColClears * 100) +
               (bs.almostFullRows * 50) +          // Medium priority for almost full
               (bs.almostFullCols * 50) +
               (bs.emptyCells * 2) +               // More empty = more flexibility
               (bs.centerControl * 3) -            // Slight bonus for center control
               (bs.edgeSpread * 1);                // Slight penalty for edge clutter
    
    return bs;
}

// Struct to hold information about cleared rows/columns
struct ClearedBoardInfo {
    Board  clearedBoard;        // The board state after clearing
    int    clearedRows;         // Number of full rows cleared
    int    clearedCols;         // Number of full columns cleared  
    int    totalClearedCells;   // Total cells cleared (accounting for overlaps)
    int    comboCount;          // Number of row+column intersections (combos)
    std::vector<int> clearedRowIndices;  // Which rows were cleared
    std::vector<int> clearedColIndices;  // Which columns were cleared
    
    ClearedBoardInfo() : clearedRows(0), clearedCols(0), totalClearedCells(0), comboCount(0) {}
};

// Static helper function to blast a board silently (without output) - used for simulation
ClearedBoardInfo blastBoardSilent(Board& board) {
    ClearedBoardInfo info;
    info.clearedBoard = board;
    
    const int boardSize = board.getBoardSize();
    std::vector<int> fullRows;
    std::vector<int> fullCols;
    
    // Detect all full rows
    for (int row = 0; row < boardSize; ++row) {
        bool isFull = true;
        for (int col = 0; col < boardSize; ++col) {
            if (!board.isCellFilled(row, col)) {
                isFull = false;
                break;
            }
        }
        if (isFull) fullRows.push_back(row);
    }
    
    // Detect all full columns
    for (int col = 0; col < boardSize; ++col) {
        bool isFull = true;
        for (int row = 0; row < boardSize; ++row) {
            if (!board.isCellFilled(row, col)) {
                isFull = false;
                break;
            }
        }
        if (isFull) fullCols.push_back(col);
    }
    
    // Calculate combos
    int comboCount = fullRows.size() * fullCols.size();
    
    // Track all cells to be cleared
    std::set<std::pair<int, int>> cellsToClear;
    for (int row : fullRows) {
        for (int col = 0; col < boardSize; ++col) {
            cellsToClear.insert({row, col});
        }
    }
    for (int col : fullCols) {
        for (int row = 0; row < boardSize; ++row) {
            cellsToClear.insert({row, col});
        }
    }
    
    // Clear all detected cells
    for (const auto& cell : cellsToClear) {
        board.removeBlock(cell.first, cell.second);
    }
    
    info.clearedRows = static_cast<int>(fullRows.size());
    info.clearedCols = static_cast<int>(fullCols.size());
    info.totalClearedCells = static_cast<int>(cellsToClear.size());
    info.comboCount = comboCount;
    info.clearedRowIndices = fullRows;
    info.clearedColIndices = fullCols;
    info.clearedBoard = board;
    
    return info;
}

class BlockBlastGame {
    private:
        Board board;

    public:
        BlockBlastGame() {}
        
        void getEmptyCells() const {
            board.printEmptyCells();
        }

        void getEmptyCellCount() const {
            std::cout << "Total Empty Cells: " << board.getEmptyCellCount() << std::endl;
        }

        void displayBoard(bool visualMode = true) const {
            board.display(visualMode);
        }
        
        void randomlyPlaceBlocks() {
            board.randomlyPlaceBlocks();
        }
        
        void start() {
            randomlyPlaceBlocks();
            displayBoard(true);
        }

        bool placeBoxOnBoard(const Box& box, int startX, int startY) {
            // Capture start time (like Go's defer pattern)
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Lambda to calculate and print duration (similar to defer)
            auto printDuration = [&startTime](bool success) {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                double durationMicroSecond = duration.count();
                std::cout << PrintUtils::YELLOW << "[ Duration: " << durationMicroSecond << " μs ]" << PrintUtils::RESET << std::endl;
            };
            
            // Phase 1: Validate all cells before placing any
            for (const auto& coord : box.getBody()) {
                int x = startX + coord.getX();
                int y = startY + coord.getY();
                if (!board.isInBounds(x, y) || board.isCellFilled(x, y)) {
                    std::cout << PrintUtils::RED << "Cannot place box " << box.getName() << " at (" << startX << ", " << startY << ")." << PrintUtils::RESET << std::endl;
                    std::cout << PrintUtils::CYAN << "Rollback any changes made." << PrintUtils::RESET << std::endl;
                    printDuration(false);
                    return false;
                }
            }
            
            // Phase 2: All validations passed, now place all cells
            for (const auto& coord : box.getBody()) {
                int x = startX + coord.getX();
                int y = startY + coord.getY();
                board.placeBlock(x, y);
            }
            std::cout << PrintUtils::GREEN << "Successfully placed box " << box.getName() << " at (" << startX << ", " << startY << ")." << PrintUtils::RESET << std::endl;
            printDuration(true);
            return true;
        }

        Board BruteForceAlgorithm(const Box& box) {
            // Capture start time
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Lambda to calculate and print duration (similar to defer)
            auto printDuration = [&startTime]() {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
                std::cout << PrintUtils::YELLOW << "[ Duration: " << duration.count() << " μs ]" << PrintUtils::RESET << std::endl;
            };
            
            Board tempBoard = board;
            for (int row = 0; row < board.getBoardSize(); ++row) {
                for (int col = 0; col < board.getBoardSize(); ++col) {
                    bool canPlace = true;
                    for (const auto& coord : box.getBody()) {
                        int x = row + coord.getX();
                        int y = col + coord.getY();
                        if (!tempBoard.isInBounds(x, y) || tempBoard.isCellFilled(x, y)) {
                            canPlace = false;
                            break;
                        }
                    }
                    if (canPlace) {
                        // Collect the coordinates that will be placed
                        std::vector<std::pair<int, int>> placedCoords;
                        for (const auto& coord : box.getBody()) {
                            int x = row + coord.getX();
                            int y = col + coord.getY();
                            tempBoard.placeBlock(x, y);
                            placedCoords.push_back({x, y});
                        }
                        
                        std::cout << PrintUtils::GREEN << "[BruteForce] Successfully placed " << box.getName() << " at (" << row << ", " << col << ")." << PrintUtils::RESET << std::endl;
                        printDuration();
                        
                        // Print board with placed cells highlighted in red
                        std::cout << "\n[Result Board - Red = Newly Placed]" << std::endl;
                        printBoardWithHighlight(tempBoard, placedCoords);
                        
                        return tempBoard;
                    }
                }
            }
            std::cout << PrintUtils::RED << "[BruteForce] Could not place " << box.getName() << " on the board." << PrintUtils::RESET << std::endl;
            printDuration();
            return board; // Return original board if no placement found
        }

        std::vector<Board> GenerateAllPossibleBoards(const Box& box) {
            // Capture start time
            auto startTime = std::chrono::high_resolution_clock::now();
            
            std::vector<Board> possibleBoards;
            std::vector<std::vector<std::pair<int, int>>> allPlacedCoords; // Track placed coords for each board
            
            for (int row = 0; row < board.getBoardSize(); ++row) {
                for (int col = 0; col < board.getBoardSize(); ++col) {
                    bool canPlace = true;
                    for (const auto& coord : box.getBody()) {
                        int x = row + coord.getX();
                        int y = col + coord.getY();
                        if (!board.isInBounds(x, y) || board.isCellFilled(x, y)) {
                            canPlace = false;
                            break;
                        }
                    }
                    if (canPlace) {
                        Board newBoard = board;
                        std::vector<std::pair<int, int>> placedCoords;
                        for (const auto& coord : box.getBody()) {
                            int x = row + coord.getX();
                            int y = col + coord.getY();
                            newBoard.placeBlock(x, y);
                            placedCoords.push_back({x, y});
                        }
                        possibleBoards.push_back(newBoard);
                        allPlacedCoords.push_back(placedCoords);
                    }
                }
            }
            
            // Calculate duration
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            
            // Print results with colors
            std::cout << "\n" << PrintUtils::CYAN << "========== GENERATE ALL POSSIBLE BOARDS ==========" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::GREEN << "✓ Box: " << box.getName() << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::BLUE << "◆ Total Possible Placements: " << possibleBoards.size() << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::GRAY << "⏱ Duration: " << duration.count() << " μs" << PrintUtils::RESET << "\n";
            
            if (possibleBoards.empty()) {
                std::cout << PrintUtils::YELLOW << "⚠ No valid placements found for this box." << PrintUtils::RESET << "\n";
            } else {
                std::cout << "\n" << PrintUtils::MAGENTA << "Preview of all " << possibleBoards.size() << " possible placements:" << PrintUtils::RESET << "\n";
                for (size_t i = 0; i < possibleBoards.size(); ++i) {
                    std::cout << "\n" << PrintUtils::YELLOW << "[Placement #" << (i + 1) << "]" << PrintUtils::RESET << "\n";
                    printBoardWithHighlight(possibleBoards[i], allPlacedCoords[i]);
                }
            }
            
            std::cout << PrintUtils::CYAN << "==================================================" << PrintUtils::RESET << "\n\n";
            
            return possibleBoards;
        }

        std::vector<Board> GenerateAllPossibleBoardsByManyPiecesCombination(const std::vector<Box>& boxes, bool verbose = false) {
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Color palette for different pieces
            std::vector<std::string> pieceColors = {
                PrintUtils::RED, PrintUtils::GREEN, PrintUtils::YELLOW,
                PrintUtils::BLUE, PrintUtils::MAGENTA, PrintUtils::CYAN
            };
            
            // Store the original board for color reference
            Board originalBoard = board;
            
            // Track boards and their piece coordinates together
            // Each entry: {board, vector of coords for each piece}
            std::vector<std::pair<Board, std::vector<std::vector<std::pair<int, int>>>>> currentBoardsWithCoords;
            currentBoardsWithCoords.push_back({board, {}});
            
            std::vector<size_t> boardsPerStep;  // Track how many boards generated at each step
            
            std::cout << "\n" << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "    GENERATING ALL POSSIBLE BOARDS BY MULTI-PIECE COMBINATION        " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            // Print pieces being placed
            std::cout << PrintUtils::GRAY << "Pieces to place: " << PrintUtils::RESET;
            for (size_t i = 0; i < boxes.size(); ++i) {
                std::string color = pieceColors[i % pieceColors.size()];
                std::cout << color << "[" << (i+1) << "] " << boxes[i].getName() << PrintUtils::RESET;
                if (i < boxes.size() - 1) std::cout << " → ";
            }
            std::cout << "\n\n";
            
            // Generate boards for each piece sequentially
            for (size_t boxIdx = 0; boxIdx < boxes.size(); ++boxIdx) {
                const auto& box = boxes[boxIdx];
                std::string color = pieceColors[boxIdx % pieceColors.size()];
                
                std::vector<std::pair<Board, std::vector<std::vector<std::pair<int, int>>>>> newBoardsWithCoords;
                size_t prevCount = currentBoardsWithCoords.size();
                
                for (const auto& boardWithCoords : currentBoardsWithCoords) {
                    const Board& currentBoard = boardWithCoords.first;
                    const std::vector<std::vector<std::pair<int, int>>>& existingCoords = boardWithCoords.second;
                    
                    // Try placing the box at every valid position
                    for (int row = 0; row < currentBoard.getBoardSize(); ++row) {
                        for (int col = 0; col < currentBoard.getBoardSize(); ++col) {
                            bool canPlace = true;
                            for (const auto& coord : box.getBody()) {
                                int x = row + coord.getX();
                                int y = col + coord.getY();
                                if (!currentBoard.isInBounds(x, y) || currentBoard.isCellFilled(x, y)) {
                                    canPlace = false;
                                    break;
                                }
                            }
                            if (canPlace) {
                                Board newBoard = currentBoard;
                                std::vector<std::pair<int, int>> newPieceCoords;
                                
                                for (const auto& coord : box.getBody()) {
                                    int x = row + coord.getX();
                                    int y = col + coord.getY();
                                    newBoard.placeBlock(x, y);
                                    newPieceCoords.push_back({x, y});
                                }
                                
                                // Copy existing coords and add new piece coords
                                std::vector<std::vector<std::pair<int, int>>> allCoords = existingCoords;
                                allCoords.push_back(newPieceCoords);
                                
                                newBoardsWithCoords.push_back({newBoard, allCoords});
                            }
                        }
                    }
                }
                
                currentBoardsWithCoords = newBoardsWithCoords;
                boardsPerStep.push_back(currentBoardsWithCoords.size());
                
                // Print step statistics with piece color
                std::cout << color << "┌─ Step " << (boxIdx + 1) << ": " << box.getName() << PrintUtils::RESET << "\n";
                std::cout << color << "│" << PrintUtils::RESET << "  Input boards: " << prevCount << "\n";
                std::cout << color << "│" << PrintUtils::RESET << "  Generated: " << PrintUtils::GREEN << currentBoardsWithCoords.size() << PrintUtils::RESET << " possible boards\n";
                if (currentBoardsWithCoords.empty()) {
                    std::cout << color << "│" << PrintUtils::RESET << "  " << PrintUtils::YELLOW << "⚠ No valid placements - stopping" << PrintUtils::RESET << "\n";
                    break;
                }
                std::cout << color << "└─────────────────────────────────" << PrintUtils::RESET << "\n\n";
            }
            
            // Extract just the boards for the return value
            std::vector<Board> currentBoards;
            for (const auto& bwc : currentBoardsWithCoords) {
                currentBoards.push_back(bwc.first);
            }
            
            // Calculate duration
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            
            // Calculate scores for all boards and attach piece coordinates
            std::vector<BoardScore> scores;
            for (size_t i = 0; i < currentBoardsWithCoords.size(); ++i) {
                BoardScore bs = calculateBoardScore(currentBoardsWithCoords[i].first, i);
                bs.pieceCoords = currentBoardsWithCoords[i].second;  // Attach piece coordinates
                scores.push_back(bs);
            }
            
            // Sort by score (highest first)
            std::sort(scores.begin(), scores.end(), [](const BoardScore& a, const BoardScore& b) {
                return a.score > b.score;
            });
            
            // Print summary statistics
            std::cout << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                         GENERATION SUMMARY                           " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╠══════════════════════════════════════════════════════════════════════╣" << PrintUtils::RESET << "\n";
            
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::BLUE << "◆ Total Pieces Placed: " << PrintUtils::RESET << boxes.size() << std::string(45 - std::to_string(boxes.size()).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::GREEN << "◆ Total Possible Boards: " << PrintUtils::RESET << currentBoards.size() << std::string(43 - std::to_string(currentBoards.size()).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::GRAY << "⏱ Generation Time: " << PrintUtils::RESET << duration.count() << " ms" << std::string(45 - std::to_string(duration.count()).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            
            if (!scores.empty()) {
                // Score statistics
                int maxScore = scores.front().score;
                int minScore = scores.back().score;
                int avgScore = 0;
                for (const auto& s : scores) avgScore += s.score;
                avgScore = scores.size() > 0 ? avgScore / static_cast<int>(scores.size()) : 0;
                
                std::cout << PrintUtils::CYAN << "╠══════════════════════════════════════════════════════════════════════╣" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                         SCORE STATISTICS                             " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::CYAN << "╠══════════════════════════════════════════════════════════════════════╣" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::GREEN << "▲ Highest Score: " << PrintUtils::RESET << maxScore << std::string(51 - std::to_string(maxScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::YELLOW << "● Average Score: " << PrintUtils::RESET << avgScore << std::string(51 - std::to_string(avgScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::RED << "▼ Lowest Score: " << PrintUtils::RESET << minScore << std::string(52 - std::to_string(minScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            }
            
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            // Show top 3 boards with detailed stats and multi-color piece visualization
            if (!scores.empty()) {
                size_t topCount = std::min(scores.size(), static_cast<size_t>(3));
                
                std::cout << PrintUtils::MAGENTA << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "                      🏆 TOP " << topCount << " HIGHEST SCORING BOARDS 🏆                  " << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::MAGENTA << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                
                std::string medals[] = {"🥇", "🥈", "🥉"};
                std::string rankColors[] = {PrintUtils::YELLOW, PrintUtils::GRAY, "\033[38;5;166m"}; // Gold, Silver, Bronze
                
                for (size_t i = 0; i < topCount; ++i) {
                    const BoardScore& bs = scores[i];
                    std::string rankColor = rankColors[i];
                    
                    std::cout << rankColor << "┌─────────────────────────────────────────────────────────────────────┐" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << " " << medals[i] << " " << rankColor << "RANK #" << (i + 1) << PrintUtils::RESET << " (Board Index: " << bs.boardIndex << ")" << std::string(40 - std::to_string(bs.boardIndex).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "├─────────────────────────────────────────────────────────────────────┤" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  " << PrintUtils::GREEN << "★ Score: " << bs.score << PrintUtils::RESET << std::string(58 - std::to_string(bs.score).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  ├─ Empty Cells: " << bs.emptyCells << " / 64" << std::string(43 - std::to_string(bs.emptyCells).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  ├─ Potential Row Clears: " << bs.potentialRowClears << std::string(40 - std::to_string(bs.potentialRowClears).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  ├─ Potential Col Clears: " << bs.potentialColClears << std::string(40 - std::to_string(bs.potentialColClears).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  ├─ Almost Full Rows (6+): " << bs.almostFullRows << std::string(39 - std::to_string(bs.almostFullRows).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  ├─ Almost Full Cols (6+): " << bs.almostFullCols << std::string(39 - std::to_string(bs.almostFullCols).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  └─ Center Control (4x4): " << bs.centerControl << " / 16" << std::string(36 - std::to_string(bs.centerControl).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "├─────────────────────────────────────────────────────────────────────┤" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << " Board Visual (";
                    std::cout << PrintUtils::RED << "Piece1" << PrintUtils::RESET << ", ";
                    std::cout << PrintUtils::GREEN << "Piece2" << PrintUtils::RESET << ", ";
                    std::cout << PrintUtils::YELLOW << "Piece3" << PrintUtils::RESET << ", Original=#):        " << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "└─────────────────────────────────────────────────────────────────────┘" << PrintUtils::RESET << "\n";
                    
                    // Print the board with multi-color piece highlighting
                    printBoardWithMultiPieceHighlight(bs.board, bs.pieceCoords, originalBoard);
                    std::cout << "\n";
                }
            }
            
            if (currentBoards.empty()) {
                std::cout << PrintUtils::YELLOW << "⚠ No valid placements found for the given combination of boxes." << PrintUtils::RESET << "\n\n";
            }
            
            return currentBoards;
        }

        // Method that returns the top 3 BoardScore objects for user selection
        std::vector<BoardScore> GetTop3BoardsWithScores(const std::vector<Box>& boxes) {
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Color palette for different pieces
            std::vector<std::string> pieceColors = {
                PrintUtils::RED, PrintUtils::GREEN, PrintUtils::YELLOW,
                PrintUtils::BLUE, PrintUtils::MAGENTA, PrintUtils::CYAN
            };
            
            // Store the original board for color reference
            Board originalBoard = board;
            
            // Track boards and their piece coordinates together
            std::vector<std::pair<Board, std::vector<std::vector<std::pair<int, int>>>>> currentBoardsWithCoords;
            currentBoardsWithCoords.push_back({board, {}});
            
            std::cout << "\n" << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "              ANALYZING ALL POSSIBLE PLACEMENTS...                    " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            // Print pieces being placed
            std::cout << PrintUtils::GRAY << "Pieces to place: " << PrintUtils::RESET;
            for (size_t i = 0; i < boxes.size(); ++i) {
                std::string color = pieceColors[i % pieceColors.size()];
                std::cout << color << "[" << (i+1) << "] " << boxes[i].getName() << PrintUtils::RESET;
                if (i < boxes.size() - 1) std::cout << " → ";
            }
            std::cout << "\n\n";
            
            // Generate boards for each piece sequentially
            for (size_t boxIdx = 0; boxIdx < boxes.size(); ++boxIdx) {
                const auto& box = boxes[boxIdx];
                std::string color = pieceColors[boxIdx % pieceColors.size()];
                
                std::vector<std::pair<Board, std::vector<std::vector<std::pair<int, int>>>>> newBoardsWithCoords;
                size_t prevCount = currentBoardsWithCoords.size();
                
                for (const auto& boardWithCoords : currentBoardsWithCoords) {
                    const Board& currentBoard = boardWithCoords.first;
                    const std::vector<std::vector<std::pair<int, int>>>& existingCoords = boardWithCoords.second;
                    
                    for (int row = 0; row < currentBoard.getBoardSize(); ++row) {
                        for (int col = 0; col < currentBoard.getBoardSize(); ++col) {
                            bool canPlace = true;
                            for (const auto& coord : box.getBody()) {
                                int x = row + coord.getX();
                                int y = col + coord.getY();
                                if (!currentBoard.isInBounds(x, y) || currentBoard.isCellFilled(x, y)) {
                                    canPlace = false;
                                    break;
                                }
                            }
                            if (canPlace) {
                                Board newBoard = currentBoard;
                                std::vector<std::pair<int, int>> newPieceCoords;
                                
                                for (const auto& coord : box.getBody()) {
                                    int x = row + coord.getX();
                                    int y = col + coord.getY();
                                    newBoard.placeBlock(x, y);
                                    newPieceCoords.push_back({x, y});
                                }
                                
                                std::vector<std::vector<std::pair<int, int>>> allCoords = existingCoords;
                                allCoords.push_back(newPieceCoords);
                                newBoardsWithCoords.push_back({newBoard, allCoords});
                            }
                        }
                    }
                }
                
                currentBoardsWithCoords = newBoardsWithCoords;
                
                std::cout << color << "Step " << (boxIdx + 1) << ": " << box.getName() << PrintUtils::RESET;
                std::cout << " → " << PrintUtils::GREEN << currentBoardsWithCoords.size() << PrintUtils::RESET << " boards\n";
                
                if (currentBoardsWithCoords.empty()) {
                    std::cout << PrintUtils::YELLOW << "⚠ No valid placements found!" << PrintUtils::RESET << "\n";
                    return {};
                }
            }
            
            // Calculate duration
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            
            // Calculate scores for all boards
            std::vector<BoardScore> scores;
            for (size_t i = 0; i < currentBoardsWithCoords.size(); ++i) {
                BoardScore bs = calculateBoardScore(currentBoardsWithCoords[i].first, i);
                bs.pieceCoords = currentBoardsWithCoords[i].second;
                scores.push_back(bs);
            }
            
            // Sort by score (highest first)
            std::sort(scores.begin(), scores.end(), [](const BoardScore& a, const BoardScore& b) {
                return a.score > b.score;
            });
            
            std::cout << "\n" << PrintUtils::GRAY << "⏱ Analysis Time: " << duration.count() << " ms | Total Combinations: " << scores.size() << PrintUtils::RESET << "\n\n";
            
            // Return top 3 (or less if not enough)
            size_t topCount = std::min(scores.size(), static_cast<size_t>(3));
            std::vector<BoardScore> top3(scores.begin(), scores.begin() + topCount);
            
            // Display top 3 for user selection
            if (!top3.empty()) {
                std::cout << PrintUtils::MAGENTA << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "                  🏆 TOP " << topCount << " RECOMMENDED PLACEMENTS 🏆                   " << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::MAGENTA << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                
                std::string medals[] = {"🥇", "🥈", "🥉"};
                std::string rankColors[] = {PrintUtils::YELLOW, PrintUtils::GRAY, "\033[38;5;166m"};
                
                for (size_t i = 0; i < topCount; ++i) {
                    const BoardScore& bs = top3[i];
                    std::string rankColor = rankColors[i];
                    
                    std::cout << rankColor << "┌─────────────────────────────────────────────────────────────────────┐" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << " " << medals[i] << " " << rankColor << "OPTION " << (i + 1) << PrintUtils::RESET << " - Score: " << PrintUtils::GREEN << bs.score << PrintUtils::RESET << std::string(44 - std::to_string(bs.score).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "├─────────────────────────────────────────────────────────────────────┤" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  Empty: " << bs.emptyCells << "/64 | Rows to clear: " << bs.potentialRowClears << " | Cols to clear: " << bs.potentialColClears << std::string(15, ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "├─────────────────────────────────────────────────────────────────────┤" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << " " << PrintUtils::RED << "■" << PrintUtils::RESET << "=Piece1 " << PrintUtils::GREEN << "■" << PrintUtils::RESET << "=Piece2 " << PrintUtils::YELLOW << "■" << PrintUtils::RESET << "=Piece3 " << "[#]=Original                  " << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "└─────────────────────────────────────────────────────────────────────┘" << PrintUtils::RESET << "\n";
                    
                    printBoardWithMultiPieceHighlight(bs.board, bs.pieceCoords, originalBoard);
                    std::cout << "\n";
                }
            }
            
            return top3;
        }

        // Silent version without printing (for internal use)
        std::vector<Board> GenerateAllPossibleBoardsSilent(const Box& box) {
            std::vector<Board> possibleBoards;
            
            for (int row = 0; row < board.getBoardSize(); ++row) {
                for (int col = 0; col < board.getBoardSize(); ++col) {
                    bool canPlace = true;
                    for (const auto& coord : box.getBody()) {
                        int x = row + coord.getX();
                        int y = col + coord.getY();
                        if (!board.isInBounds(x, y) || board.isCellFilled(x, y)) {
                            canPlace = false;
                            break;
                        }
                    }
                    if (canPlace) {
                        Board newBoard = board;
                        for (const auto& coord : box.getBody()) {
                            int x = row + coord.getX();
                            int y = col + coord.getY();
                            newBoard.placeBlock(x, y);
                        }
                        possibleBoards.push_back(newBoard);
                    }
                }
            }
            return possibleBoards;
        }

        // NEW: Generate all placement sequences with blasting after each piece
        // This considers all permutations and blasts after each placement
        std::vector<PlacementSequence> GenerateAllPlacementsWithBlasting(const std::vector<Box>& boxes) {
            std::vector<PlacementSequence> allSequences;
            
            if (boxes.empty()) return allSequences;
            
            // Generate all permutations of piece indices
            std::vector<int> indices;
            for (size_t i = 0; i < boxes.size(); ++i) {
                indices.push_back(static_cast<int>(i));
            }
            
            // Sort indices to prepare for permutation generation
            std::sort(indices.begin(), indices.end());
            
            do {
                // For this permutation, generate all possible placement sequences
                // Start with the current board state
                std::vector<PlacementSequence> currentSequences;
                
                // Initialize with empty sequence
                PlacementSequence initial;
                initial.finalBoard = board;
                initial.pieceOrder = indices;
                initial.totalCellsCleared = 0;
                initial.totalLinesCleared = 0;
                currentSequences.push_back(initial);
                
                // Process each piece in the permutation order
                for (int pieceIdx : indices) {
                    const Box& currentBox = boxes[pieceIdx];
                    std::vector<PlacementSequence> newSequences;
                    
                    for (const PlacementSequence& seq : currentSequences) {
                        // Try placing the piece at every valid position on the current board
                        for (int row = 0; row < seq.finalBoard.getBoardSize(); ++row) {
                            for (int col = 0; col < seq.finalBoard.getBoardSize(); ++col) {
                                // Check if piece can be placed
                                bool canPlace = true;
                                for (const auto& coord : currentBox.getBody()) {
                                    int x = row + coord.getX();
                                    int y = col + coord.getY();
                                    if (!seq.finalBoard.isInBounds(x, y) || seq.finalBoard.isCellFilled(x, y)) {
                                        canPlace = false;
                                        break;
                                    }
                                }
                                
                                if (canPlace) {
                                    // Create new sequence based on current
                                    PlacementSequence newSeq = seq;
                                    
                                    // Place the piece
                                    PlacementStep step;
                                    step.pieceIndex = pieceIdx;
                                    step.position = {row, col};
                                    
                                    for (const auto& coord : currentBox.getBody()) {
                                        int x = row + coord.getX();
                                        int y = col + coord.getY();
                                        newSeq.finalBoard.placeBlock(x, y);
                                        step.pieceCoords.push_back({x, y});
                                    }
                                    
                                    // BLAST the board after placement
                                    ClearedBoardInfo blastResult = blastBoardSilent(newSeq.finalBoard);
                                    
                                    // Record blast results in the step
                                    step.clearedRows = blastResult.clearedRows;
                                    step.clearedCols = blastResult.clearedCols;
                                    step.cellsCleared = blastResult.totalClearedCells;
                                    
                                    // Accumulate totals
                                    newSeq.totalCellsCleared += blastResult.totalClearedCells;
                                    newSeq.totalLinesCleared += blastResult.clearedRows + blastResult.clearedCols;
                                    
                                    // Add step to sequence
                                    newSeq.steps.push_back(step);
                                    
                                    newSequences.push_back(newSeq);
                                }
                            }
                        }
                    }
                    
                    currentSequences = newSequences;
                    
                    // Early termination if no valid placements
                    if (currentSequences.empty()) break;
                }
                
                // Add all completed sequences from this permutation
                for (auto& seq : currentSequences) {
                    allSequences.push_back(seq);
                }
                
            } while (std::next_permutation(indices.begin(), indices.end()));
            
            return allSequences;
        }
        
        // Calculate score for a PlacementSequence
        BoardScore calculateSequenceScore(const PlacementSequence& seq, size_t index, const std::vector<Box>& boxes) {
            BoardScore bs;
            bs.board = seq.finalBoard;
            bs.boardIndex = index;
            bs.totalCellsCleared = seq.totalCellsCleared;
            bs.totalLinesCleared = seq.totalLinesCleared;
            bs.pieceOrder = seq.pieceOrder;
            
            // Extract piece positions and coords from steps
            for (const auto& step : seq.steps) {
                bs.piecePositions.push_back(step.position);
                bs.pieceCoords.push_back(step.pieceCoords);
            }
            
            const int boardSize = seq.finalBoard.getBoardSize();
            bs.emptyCells = seq.finalBoard.getEmptyCellCount();
            
            // Check each row for potential future clears
            for (int row = 0; row < boardSize; ++row) {
                int filledCount = 0;
                for (int col = 0; col < boardSize; ++col) {
                    if (seq.finalBoard.isCellFilled(row, col)) filledCount++;
                }
                if (filledCount == boardSize) bs.potentialRowClears++;
                else if (filledCount >= 7) bs.potentialRowClears++;
                else if (filledCount >= 6) bs.almostFullRows++;
            }
            
            // Check each column for potential future clears
            for (int col = 0; col < boardSize; ++col) {
                int filledCount = 0;
                for (int row = 0; row < boardSize; ++row) {
                    if (seq.finalBoard.isCellFilled(row, col)) filledCount++;
                }
                if (filledCount == boardSize) bs.potentialColClears++;
                else if (filledCount >= 7) bs.potentialColClears++;
                else if (filledCount >= 6) bs.almostFullCols++;
            }
            
            // Calculate center control
            for (int row = 2; row < 6; ++row) {
                for (int col = 2; col < 6; ++col) {
                    if (seq.finalBoard.isCellFilled(row, col)) bs.centerControl++;
                }
            }
            
            // Calculate edge spread
            for (int row = 0; row < boardSize; ++row) {
                for (int col = 0; col < boardSize; ++col) {
                    if (seq.finalBoard.isCellFilled(row, col)) {
                        if (row == 0 || row == boardSize-1 || col == 0 || col == boardSize-1) {
                            bs.edgeSpread++;
                        }
                    }
                }
            }
            
            // NEW SCORING: Prioritize cleared cells, then board quality
            // Cells cleared during sequence is most important (actual points earned)
            bs.score = (bs.totalCellsCleared * 15) +          // HIGH: Actual cleared cells
                       (bs.totalLinesCleared * 80) +          // HIGH: Lines cleared (bonus)
                       (bs.potentialRowClears * 60) +         // Setup for future clears
                       (bs.potentialColClears * 60) +
                       (bs.almostFullRows * 30) +
                       (bs.almostFullCols * 30) +
                       (bs.emptyCells * 2) +                  // More empty = more flexibility
                       (bs.centerControl * 2) -               // Slight bonus for center
                       (bs.edgeSpread * 1);                   // Slight penalty for edges
            
            return bs;
        }
        
        // NEW: Get top 3 placements considering all permutations with blasting
        std::vector<BoardScore> GetTop3BoardsWithBlasting(const std::vector<Box>& boxes) {
            auto startTime = std::chrono::high_resolution_clock::now();
            
            // Color palette for different pieces
            std::vector<std::string> pieceColors = {
                PrintUtils::RED, PrintUtils::GREEN, PrintUtils::YELLOW,
                PrintUtils::BLUE, PrintUtils::MAGENTA, PrintUtils::CYAN
            };
            
            // Store the original board for display
            Board originalBoard = board;
            
            std::cout << "\n" << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "         🧠 SMART BOT: ANALYZING ALL PLACEMENTS WITH BLASTING         " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            // Print pieces
            std::cout << PrintUtils::GRAY << "📦 Available pieces: " << PrintUtils::RESET;
            for (size_t i = 0; i < boxes.size(); ++i) {
                std::string color = pieceColors[i % pieceColors.size()];
                std::cout << color << "[" << (i+1) << "] " << boxes[i].getName() << PrintUtils::RESET;
                if (i < boxes.size() - 1) std::cout << ", ";
            }
            std::cout << "\n";
            
            // Show permutation info
            int numPermutations = 1;
            for (size_t i = 1; i <= boxes.size(); ++i) numPermutations *= i;
            std::cout << PrintUtils::GRAY << "🔄 Considering " << numPermutations << " piece orderings (any order allowed)" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::GRAY << "💥 Blasting after EACH piece placement" << PrintUtils::RESET << "\n\n";
            
            // Generate all placement sequences
            std::vector<PlacementSequence> allSequences = GenerateAllPlacementsWithBlasting(boxes);
            
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            
            if (allSequences.empty()) {
                std::cout << PrintUtils::RED << "⚠ No valid placement sequences found!" << PrintUtils::RESET << "\n";
                return {};
            }
            
            std::cout << PrintUtils::GREEN << "✓ Generated " << allSequences.size() << " possible sequences" << PrintUtils::RESET << "\n";
            
            // Calculate scores for all sequences
            std::vector<BoardScore> scores;
            for (size_t i = 0; i < allSequences.size(); ++i) {
                BoardScore bs = calculateSequenceScore(allSequences[i], i, boxes);
                scores.push_back(bs);
            }
            
            // Sort by score (highest first)
            std::sort(scores.begin(), scores.end(), [](const BoardScore& a, const BoardScore& b) {
                return a.score > b.score;
            });
            
            // Statistics
            int maxScore = scores.front().score;
            int minScore = scores.back().score;
            int avgScore = 0;
            int maxCleared = 0;
            for (const auto& s : scores) {
                avgScore += s.score;
                if (s.totalCellsCleared > maxCleared) maxCleared = s.totalCellsCleared;
            }
            avgScore /= static_cast<int>(scores.size());
            
            std::cout << "\n" << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                        ANALYSIS STATISTICS                           " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╠══════════════════════════════════════════════════════════════════════╣" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  ⏱ Analysis Time: " << duration.count() << " ms" << std::string(48 - std::to_string(duration.count()).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  📊 Total Sequences: " << scores.size() << std::string(46 - std::to_string(scores.size()).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::GREEN << "▲ Best Score: " << maxScore << PrintUtils::RESET << std::string(54 - std::to_string(maxScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::YELLOW << "● Average Score: " << avgScore << PrintUtils::RESET << std::string(51 - std::to_string(avgScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  " << PrintUtils::RED << "▼ Lowest Score: " << minScore << PrintUtils::RESET << std::string(52 - std::to_string(minScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "  💥 Max Cells Cleared: " << maxCleared << std::string(44 - std::to_string(maxCleared).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            // Return top 3
            size_t topCount = std::min(scores.size(), static_cast<size_t>(3));
            std::vector<BoardScore> top3(scores.begin(), scores.begin() + topCount);
            
            // Display top 3
            if (!top3.empty()) {
                std::cout << PrintUtils::MAGENTA << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "                  🏆 TOP " << topCount << " RECOMMENDED SEQUENCES 🏆                   " << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "\n";
                std::cout << PrintUtils::MAGENTA << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
                
                std::string medals[] = {"🥇", "🥈", "🥉"};
                std::string rankColors[] = {PrintUtils::YELLOW, PrintUtils::GRAY, "\033[38;5;166m"};
                
                for (size_t i = 0; i < topCount; ++i) {
                    const BoardScore& bs = top3[i];
                    std::string rankColor = rankColors[i];
                    
                    std::cout << rankColor << "┌─────────────────────────────────────────────────────────────────────┐" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << " " << medals[i] << " " << rankColor << "OPTION " << (i + 1) << PrintUtils::RESET << " - Score: " << PrintUtils::GREEN << bs.score << PrintUtils::RESET << std::string(44 - std::to_string(bs.score).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "├─────────────────────────────────────────────────────────────────────┤" << PrintUtils::RESET << "\n";
                    
                    // Show placement order
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  📋 Placement Order: ";
                    for (size_t j = 0; j < bs.pieceOrder.size(); ++j) {
                        int pIdx = bs.pieceOrder[j];
                        std::string pColor = pieceColors[pIdx % pieceColors.size()];
                        std::cout << pColor << boxes[pIdx].getName() << PrintUtils::RESET;
                        if (j < bs.pieceOrder.size() - 1) std::cout << " → ";
                    }
                    std::cout << std::string(5, ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    
                    // Show cleared stats
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  💥 Cells Cleared: " << bs.totalCellsCleared << " | Lines: " << bs.totalLinesCleared << std::string(33 - std::to_string(bs.totalCellsCleared).length() - std::to_string(bs.totalLinesCleared).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << "  📍 Empty After: " << bs.emptyCells << "/64 | Future Clear Setup: " << (bs.potentialRowClears + bs.potentialColClears) << std::string(13 - std::to_string(bs.emptyCells).length() - std::to_string(bs.potentialRowClears + bs.potentialColClears).length(), ' ') << rankColor << "│" << PrintUtils::RESET << "\n";
                    
                    std::cout << rankColor << "├─────────────────────────────────────────────────────────────────────┤" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "│" << PrintUtils::RESET << " " << PrintUtils::RED << "■" << PrintUtils::RESET << "=Piece1 " << PrintUtils::GREEN << "■" << PrintUtils::RESET << "=Piece2 " << PrintUtils::YELLOW << "■" << PrintUtils::RESET << "=Piece3 " << "[#]=Original (order may vary)  " << rankColor << "│" << PrintUtils::RESET << "\n";
                    std::cout << rankColor << "└─────────────────────────────────────────────────────────────────────┘" << PrintUtils::RESET << "\n";
                    
                    // Print board with piece highlighting
                    // Reorder pieceCoords based on pieceOrder to show which piece went where
                    printBoardWithMultiPieceHighlight(bs.board, bs.pieceCoords, originalBoard);
                    std::cout << "\n";
                }
            }
            
            return top3;
        }

        bool useThisBoard(const Board& newBoard) {
            board = newBoard;
            return true;
        }

        ClearedBoardInfo blastTheBoard() {
            auto startTime = std::chrono::high_resolution_clock::now();
            
            ClearedBoardInfo info;
            info.clearedBoard = board;  // Store initial state
            
            const int boardSize = board.getBoardSize();
            std::vector<int> fullRows;
            std::vector<int> fullCols;
            
            // Phase 1: Detect all full rows BEFORE clearing anything
            for (int row = 0; row < boardSize; ++row) {
                bool isFull = true;
                for (int col = 0; col < boardSize; ++col) {
                    if (!board.isCellFilled(row, col)) {
                        isFull = false;
                        break;
                    }
                }
                if (isFull) {
                    fullRows.push_back(row);
                }
            }
            
            // Phase 2: Detect all full columns BEFORE clearing anything
            for (int col = 0; col < boardSize; ++col) {
                bool isFull = true;
                for (int row = 0; row < boardSize; ++row) {
                    if (!board.isCellFilled(row, col)) {
                        isFull = false;
                        break;
                    }
                }
                if (isFull) {
                    fullCols.push_back(col);
                }
            }
            
            // Phase 3: Calculate combos (intersections where row and column both clear)
            int comboCount = 0;
            std::vector<std::pair<int, int>> comboIntersections;
            for (int row : fullRows) {
                for (int col : fullCols) {
                    comboCount++;
                    comboIntersections.push_back({row, col});
                }
            }
            
            // Phase 4: Track all cells to be cleared using a set to avoid duplicates
            std::set<std::pair<int, int>> cellsToClear;
            
            // Add all cells from full rows
            for (int row : fullRows) {
                for (int col = 0; col < boardSize; ++col) {
                    cellsToClear.insert({row, col});
                }
            }
            
            // Add all cells from full columns
            for (int col : fullCols) {
                for (int row = 0; row < boardSize; ++row) {
                    cellsToClear.insert({row, col});
                }
            }
            
            // Phase 5: Clear all detected cells
            for (const auto& cell : cellsToClear) {
                board.removeBlock(cell.first, cell.second);
            }
            
            // Populate ClearedBoardInfo
            info.clearedRows = static_cast<int>(fullRows.size());
            info.clearedCols = static_cast<int>(fullCols.size());
            info.totalClearedCells = static_cast<int>(cellsToClear.size());
            info.comboCount = comboCount;
            info.clearedRowIndices = fullRows;
            info.clearedColIndices = fullCols;
            info.clearedBoard = board;  // Update to final state after clearing
            
            // Calculate duration
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            
            // Print detailed statistics with colors
            std::cout << "\n" << PrintUtils::CYAN << "========== BLAST THE BOARD RESULTS ==========" << PrintUtils::RESET << "\n";
            
            if (fullRows.empty() && fullCols.empty()) {
                std::cout << PrintUtils::YELLOW << "⚠ No full rows or columns detected." << PrintUtils::RESET << "\n";
            } else {
                // Row clearing info
                if (!fullRows.empty()) {
                    std::cout << PrintUtils::GREEN << "✓ Rows Cleared: " << info.clearedRows << PrintUtils::RESET;
                    std::cout << " → [";
                    for (size_t i = 0; i < fullRows.size(); ++i) {
                        std::cout << "Row " << fullRows[i];
                        if (i < fullRows.size() - 1) std::cout << ", ";
                    }
                    std::cout << "]\n";
                }
                
                // Column clearing info
                if (!fullCols.empty()) {
                    std::cout << PrintUtils::GREEN << "✓ Columns Cleared: " << info.clearedCols << PrintUtils::RESET;
                    std::cout << " → [";
                    for (size_t i = 0; i < fullCols.size(); ++i) {
                        std::cout << "Col " << fullCols[i];
                        if (i < fullCols.size() - 1) std::cout << ", ";
                    }
                    std::cout << "]\n";
                }
                
                // Combo info (intersections)
                if (comboCount > 0) {
                    std::cout << PrintUtils::MAGENTA << "★ COMBO x" << comboCount << "!" << PrintUtils::RESET;
                    std::cout << " (Row+Column intersections at: ";
                    for (size_t i = 0; i < comboIntersections.size(); ++i) {
                        std::cout << "(" << comboIntersections[i].first << "," << comboIntersections[i].second << ")";
                        if (i < comboIntersections.size() - 1) std::cout << ", ";
                    }
                    std::cout << ")\n";
                }
                
                // Total cells cleared
                std::cout << PrintUtils::BLUE << "◆ Total Cells Cleared: " << info.totalClearedCells << PrintUtils::RESET;
                
                // Show overlap calculation if there were combos
                if (comboCount > 0) {
                    int rawCells = (info.clearedRows * boardSize) + (info.clearedCols * boardSize);
                    int overlap = rawCells - info.totalClearedCells;
                    std::cout << " (Rows: " << info.clearedRows * boardSize 
                              << " + Cols: " << info.clearedCols * boardSize 
                              << " - Overlap: " << overlap << ")";
                }
                std::cout << "\n";
            }
            
            std::cout << PrintUtils::GRAY << "⏱ Duration: " << duration.count() << " μs" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "==============================================" << PrintUtils::RESET << "\n\n";
            
            return info;
        }
};

Box randomShapeInBundle () {
    std::vector<Box> boxes = BoxBundle();
    int randomIndex = rand() % boxes.size();
    return boxes[randomIndex];
}

// Function to display the game title and welcome screen
void displayWelcomeScreen() {
    system("clear");
    std::cout << PrintUtils::CYAN << R"(
╔═══════════════════════════════════════════════════════════════════════════╗
║                                                                           ║
║    ██████╗ ██╗      ██████╗  ██████╗██╗  ██╗    ██████╗ ██╗      █████╗   ║
║    ██╔══██╗██║     ██╔═══██╗██╔════╝██║ ██╔╝    ██╔══██╗██║     ██╔══██╗  ║
║    ██████╔╝██║     ██║   ██║██║     █████╔╝     ██████╔╝██║     ███████║  ║
║    ██╔══██╗██║     ██║   ██║██║     ██╔═██╗     ██╔══██╗██║     ██╔══██║  ║
║    ██████╔╝███████╗╚██████╔╝╚██████╗██║  ██╗    ██████╔╝███████╗██║  ██║  ║
║    ╚═════╝ ╚══════╝ ╚═════╝  ╚═════╝╚═╝  ╚═╝    ╚═════╝ ╚══════╝╚═╝  ╚═╝  ║
║                                                                           ║
║                        🎮 BLOCK BLAST BOT 🎮                              ║
║                                                                           ║
╚═══════════════════════════════════════════════════════════════════════════╝
)" << PrintUtils::RESET << "\n";
}

// Function to display turn header
void displayTurnHeader(int turnNumber, int totalScore) {
    std::cout << "\n" << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
    std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                           " << PrintUtils::YELLOW << "TURN " << turnNumber << PrintUtils::RESET << "                                     " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
    std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                      Total Score: " << PrintUtils::GREEN << totalScore << PrintUtils::RESET << std::string(32 - std::to_string(totalScore).length(), ' ') << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
    std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
}

// Function to display current board status
void displayBoardStatus(BlockBlastGame& game) {
    std::cout << PrintUtils::GRAY << "─────────────────── Current Board ───────────────────" << PrintUtils::RESET << "\n";
    game.displayBoard();
    game.getEmptyCellCount();
    std::cout << PrintUtils::GRAY << "─────────────────────────────────────────────────────" << PrintUtils::RESET << "\n\n";
}


int main() {
    srand(time(NULL));
    
    displayWelcomeScreen();
    std::cout << PrintUtils::GREEN << "Press ENTER to start the game..." << PrintUtils::RESET;
    std::cin.get();
    
    BlockBlastGame game;
    int turnNumber = 1;
    int totalScore = 0;
    int totalClearedCells = 0;
    
    // Main game loop
    while (true) {
        system("clear");
        displayTurnHeader(turnNumber, totalScore);
        displayBoardStatus(game);
        
        // Step 1: User selects 3 pieces
        std::cout << PrintUtils::MAGENTA << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
        std::cout << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "              📦 SELECT 3 PIECES FOR THIS TURN 📦                     " << PrintUtils::MAGENTA << "║" << PrintUtils::RESET << "\n";
        std::cout << PrintUtils::MAGENTA << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
        
        std::vector<Box> selectedPieces;
        std::vector<std::string> pieceColors = {PrintUtils::RED, PrintUtils::GREEN, PrintUtils::YELLOW};
        
        for (int i = 0; i < 3; ++i) {
            std::cout << pieceColors[i] << "┌─────────────────────────────────────────────────────────────────────┐" << PrintUtils::RESET << "\n";
            std::cout << pieceColors[i] << "│" << PrintUtils::RESET << "                      Select Piece #" << (i + 1) << "                                 " << pieceColors[i] << "│" << PrintUtils::RESET << "\n";
            std::cout << pieceColors[i] << "└─────────────────────────────────────────────────────────────────────┘" << PrintUtils::RESET << "\n";
            
            Box piece = MenuBoxSelection();
            selectedPieces.push_back(piece);
            
            system("clear");
            displayTurnHeader(turnNumber, totalScore);
            
            std::cout << PrintUtils::GRAY << "Selected pieces so far:" << PrintUtils::RESET << "\n";
            for (size_t j = 0; j <= static_cast<size_t>(i); ++j) {
                std::cout << pieceColors[j] << "[" << (j+1) << "] " << selectedPieces[j].getName() << PrintUtils::RESET;
                if (j < static_cast<size_t>(i)) std::cout << " → ";
            }
            std::cout << "\n\n";
        }
        
        // Step 2: Get top 3 placements (NEW: considers all permutations with blasting)
        std::vector<BoardScore> top3 = game.GetTop3BoardsWithBlasting(selectedPieces);
        
        if (top3.empty()) {
            std::cout << PrintUtils::RED << "\n╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::RED << "║" << PrintUtils::RESET << "                        💀 GAME OVER 💀                               " << PrintUtils::RED << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::RED << "║" << PrintUtils::RESET << "          No valid placements available for these pieces!             " << PrintUtils::RED << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::RED << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            std::cout << PrintUtils::YELLOW << "Final Statistics:" << PrintUtils::RESET << "\n";
            std::cout << "  Total Turns: " << turnNumber << "\n";
            std::cout << "  Total Score: " << totalScore << "\n";
            std::cout << "  Total Cells Cleared: " << totalClearedCells << "\n\n";
            break;
        }
        
        // Step 3: User selects one of the top 3
        int choice = 0;
        while (choice < 1 || choice > static_cast<int>(top3.size())) {
            std::cout << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                    🎯 SELECT YOUR PLACEMENT 🎯                       " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n";
            std::cout << "Enter your choice (1-" << top3.size() << "): ";
            std::cin >> choice;
            
            if (choice < 1 || choice > static_cast<int>(top3.size())) {
                std::cout << PrintUtils::RED << "Invalid choice! Please enter 1-" << top3.size() << PrintUtils::RESET << "\n";
            }
        }
        std::cin.ignore(); // Clear the newline
        
        // Step 4: Apply the selected board (already includes all blasting!)
        BoardScore selectedBoard = top3[choice - 1];
        game.useThisBoard(selectedBoard.board);
        
        std::cout << "\n" << PrintUtils::GREEN << "✓ Applied placement sequence #" << choice << PrintUtils::RESET << "\n";
        
        // Show placement order that was used
        std::cout << PrintUtils::GRAY << "  Piece order: " << PrintUtils::RESET;
        for (size_t i = 0; i < selectedBoard.pieceOrder.size(); ++i) {
            int pIdx = selectedBoard.pieceOrder[i];
            std::cout << pieceColors[pIdx % pieceColors.size()] << selectedPieces[pIdx].getName() << PrintUtils::RESET;
            if (i < selectedBoard.pieceOrder.size() - 1) std::cout << " → ";
        }
        std::cout << "\n";
        
        // Update statistics from the pre-calculated blast results
        int cellsClearedThisTurn = selectedBoard.totalCellsCleared;
        int linesClearedThisTurn = selectedBoard.totalLinesCleared;
        totalClearedCells += cellsClearedThisTurn;
        
        // Score calculation
        int placementScore = selectedBoard.score;
        int blastBonus = (linesClearedThisTurn * 100);  // Bonus for lines cleared during sequence
        
        if (cellsClearedThisTurn > 0) {
            std::cout << PrintUtils::MAGENTA << "\n💥 Lines cleared during sequence: " << linesClearedThisTurn << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::GREEN << "🎉 Cells cleared: " << cellsClearedThisTurn << " → +" << blastBonus << " bonus points!" << PrintUtils::RESET << "\n";
        } else {
            std::cout << PrintUtils::YELLOW << "\n⚠ No lines cleared this turn" << PrintUtils::RESET << "\n";
        }
        
        totalScore += placementScore;
        
        // Step 5: Show result (blast already happened during analysis!)
        std::cout << "\n" << PrintUtils::GRAY << "─────────────────── Board After Turn ───────────────────" << PrintUtils::RESET << "\n";
        game.displayBoard();
        game.getEmptyCellCount();
        std::cout << PrintUtils::GRAY << "─────────────────────────────────────────────────────────" << PrintUtils::RESET << "\n";
        
        std::cout << "\n" << PrintUtils::YELLOW << "Turn " << turnNumber << " Complete! Current Score: " << totalScore << PrintUtils::RESET << "\n\n";
        
        // Ask to continue
        std::cout << "Press " << PrintUtils::GREEN << "ENTER" << PrintUtils::RESET << " to continue to next turn, or type " << PrintUtils::RED << "'q'" << PrintUtils::RESET << " to quit: ";
        std::string input;
        std::getline(std::cin, input);
        
        if (input == "q" || input == "Q") {
            std::cout << "\n" << PrintUtils::CYAN << "╔══════════════════════════════════════════════════════════════════════╗" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "║" << PrintUtils::RESET << "                     🎮 THANKS FOR PLAYING! 🎮                        " << PrintUtils::CYAN << "║" << PrintUtils::RESET << "\n";
            std::cout << PrintUtils::CYAN << "╚══════════════════════════════════════════════════════════════════════╝" << PrintUtils::RESET << "\n\n";
            
            std::cout << PrintUtils::YELLOW << "Final Statistics:" << PrintUtils::RESET << "\n";
            std::cout << "  Total Turns: " << turnNumber << "\n";
            std::cout << "  Total Score: " << PrintUtils::GREEN << totalScore << PrintUtils::RESET << "\n";
            std::cout << "  Total Cells Cleared: " << totalClearedCells << "\n\n";
            break;
        }
        
        turnNumber++;
    }
    
    return 0;
}
