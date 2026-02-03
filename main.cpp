#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <set>

// Forward declaration - ClearedBoardInfo will be defined after Board class

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

void printBoxVisualOnBoard(const Box& box, int startX, int startY, int boardSize) {
    std::vector<std::vector<char>> board(boardSize, std::vector<char>(boardSize, ' '));
    for (const auto& coord : box.getBody()) {
        int x = startX + coord.getX();
        int y = startY + coord.getY();
        if (x < boardSize && y < boardSize) {
            board[x][y] = '#';
        }
    }
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            std::cout << "[" << board[i][j] << "]";
        }
        std::cout << std::endl;
    }
}

Box MenuBoxSelection() {
    system("clear");
    int mainChoice;
    int subChoice;
    
    while (true) {
        std::cout << "========================================" << std::endl;
        std::cout << "       BOX SELECTION MENU" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Please select a box type from the following options:" << std::endl;
        std::cout << "1. SquareBox" << std::endl;
        std::cout << "2. RectangleBox" << std::endl;
        std::cout << "3. LShapeBox" << std::endl;
        std::cout << "4. ConnerBox" << std::endl;
        std::cout << "5. TShapeBox" << std::endl;
        std::cout << "6. ZShapeBox" << std::endl;
        std::cout << "7. DiagonalBox" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Enter your choice (1-7): ";
        std::cin >> mainChoice;
        system("clear");
        
        switch (mainChoice) {
            case 1: {
                std::cout << "========================================" << std::endl;
                std::cout << "       SQUARE BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select size for SquareBox:" << std::endl;
                std::cout << "\n1. Size 1x1" << std::endl;
                printBoxVisualOnBoard(SquareBox(1), 0, 0, 5);
                std::cout << "\n2. Size 2x2" << std::endl;
                printBoxVisualOnBoard(SquareBox(2), 0, 0, 5);
                std::cout << "\n3. Size 3x3" << std::endl;
                printBoxVisualOnBoard(SquareBox(3), 0, 0, 5);
                std::cout << "Enter your choice (1-3): ";
                std::cin >> subChoice;
                if (subChoice >= 1 && subChoice <= 3) {
                    Box selectedBox = SquareBox(subChoice);
                    return selectedBox;
                }
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
            case 2: {
                std::cout << "========================================" << std::endl;
                std::cout << "       RECTANGLE BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select type for RectangleBox:" << std::endl;
                std::cout << "\n1. LINE 1x2" << std::endl;
                printBoxVisualOnBoard(RectangleBox(1, 2), 0, 0, 5);
                std::cout << "\n2. LINE 2x1" << std::endl;
                printBoxVisualOnBoard(RectangleBox(2, 1), 0, 0, 5);
                std::cout << "\n3. LINE 1x3" << std::endl;
                printBoxVisualOnBoard(RectangleBox(1, 3), 0, 0, 5);
                std::cout << "\n4. LINE 3x1" << std::endl;
                printBoxVisualOnBoard(RectangleBox(3, 1), 0, 0, 5);
                std::cout << "\n5. LINE 1x4" << std::endl;
                printBoxVisualOnBoard(RectangleBox(1, 4), 0, 0, 5);
                std::cout << "\n6. LINE 4x1" << std::endl;
                printBoxVisualOnBoard(RectangleBox(4, 1), 0, 0, 5);
                std::cout << "\n7. LINE 1x5" << std::endl;
                printBoxVisualOnBoard(RectangleBox(1, 5), 0, 0, 5);
                std::cout << "\n8. LINE 5x1" << std::endl;
                printBoxVisualOnBoard(RectangleBox(5, 1), 0, 0, 5);
                std::cout << "\n9. BLOCK 2x3" << std::endl;
                printBoxVisualOnBoard(RectangleBox(2, 3), 0, 0, 5);
                std::cout << "\n10. BLOCK 3x2" << std::endl;
                printBoxVisualOnBoard(RectangleBox(3, 2), 0, 0, 5);
                std::cout << "Enter your choice (1-10): ";
                std::cin >> subChoice;
                Box selectedBox = SquareBox(1); // Default
                std::string desc;
                switch (subChoice) {
                    case 1: selectedBox = RectangleBox(1, 2); desc = "LINE 1x2"; break;
                    case 2: selectedBox = RectangleBox(2, 1); desc = "LINE 2x1"; break;
                    case 3: selectedBox = RectangleBox(1, 3); desc = "LINE 1x3"; break;
                    case 4: selectedBox = RectangleBox(3, 1); desc = "LINE 3x1"; break;
                    case 5: selectedBox = RectangleBox(1, 4); desc = "LINE 1x4"; break;
                    case 6: selectedBox = RectangleBox(4, 1); desc = "LINE 4x1"; break;
                    case 7: selectedBox = RectangleBox(1, 5); desc = "LINE 1x5"; break;
                    case 8: selectedBox = RectangleBox(5, 1); desc = "LINE 5x1"; break;
                    case 9: selectedBox = RectangleBox(2, 3); desc = "BLOCK 2x3"; break;
                    case 10: selectedBox = RectangleBox(3, 2); desc = "BLOCK 3x2"; break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        continue;
                }
                return selectedBox;
            }
            case 3: {
                std::cout << "========================================" << std::endl;
                std::cout << "       L-SHAPE BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select type for LShapeBox:" << std::endl;
                std::cout << "\n1. Left L, Rotation 0" << std::endl;
                printBoxVisualOnBoard(LShapeBox("left", 0), 0, 0, 5);
                std::cout << "\n2. Left L, Rotation 90" << std::endl;
                printBoxVisualOnBoard(LShapeBox("left", 90), 0, 0, 5);
                std::cout << "\n3. Left L, Rotation 180" << std::endl;
                printBoxVisualOnBoard(LShapeBox("left", 180), 0, 0, 5);
                std::cout << "\n4. Left L, Rotation 270" << std::endl;
                printBoxVisualOnBoard(LShapeBox("left", 270), 0, 0, 5);
                std::cout << "\n5. Right L, Rotation 0" << std::endl;
                printBoxVisualOnBoard(LShapeBox("right", 0), 0, 0, 5);
                std::cout << "\n6. Right L, Rotation 90" << std::endl;
                printBoxVisualOnBoard(LShapeBox("right", 90), 0, 0, 5);
                std::cout << "\n7. Right L, Rotation 180" << std::endl;
                printBoxVisualOnBoard(LShapeBox("right", 180), 0, 0, 5);
                std::cout << "\n8. Right L, Rotation 270" << std::endl;
                printBoxVisualOnBoard(LShapeBox("right", 270), 0, 0, 5);
                std::cout << "Enter your choice (1-8): ";
                std::cin >> subChoice;
                Box selectedBox = SquareBox(1); // Default
                std::string desc;
                switch (subChoice) {
                    case 1: selectedBox = LShapeBox("left", 0); desc = "Left L, Rotation 0"; break;
                    case 2: selectedBox = LShapeBox("left", 90); desc = "Left L, Rotation 90"; break;
                    case 3: selectedBox = LShapeBox("left", 180); desc = "Left L, Rotation 180"; break;
                    case 4: selectedBox = LShapeBox("left", 270); desc = "Left L, Rotation 270"; break;
                    case 5: selectedBox = LShapeBox("right", 0); desc = "Right L, Rotation 0"; break;
                    case 6: selectedBox = LShapeBox("right", 90); desc = "Right L, Rotation 90"; break;
                    case 7: selectedBox = LShapeBox("right", 180); desc = "Right L, Rotation 180"; break;
                    case 8: selectedBox = LShapeBox("right", 270); desc = "Right L, Rotation 270"; break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        continue;
                }
                return selectedBox;
            }
            case 4: {
                std::cout << "========================================" << std::endl;
                std::cout << "       CORNER BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select type for ConnerBox:" << std::endl;
                std::cout << "\n1. Size 2, Rotation 0" << std::endl;
                printBoxVisualOnBoard(ConnerBox(2, 0), 0, 0, 5);
                std::cout << "\n2. Size 2, Rotation 90" << std::endl;
                printBoxVisualOnBoard(ConnerBox(2, 90), 0, 0, 5);
                std::cout << "\n3. Size 2, Rotation 180" << std::endl;
                printBoxVisualOnBoard(ConnerBox(2, 180), 0, 0, 5);
                std::cout << "\n4. Size 2, Rotation 270" << std::endl;
                printBoxVisualOnBoard(ConnerBox(2, 270), 0, 0, 5);
                std::cout << "\n5. Size 3, Rotation 0" << std::endl;
                printBoxVisualOnBoard(ConnerBox(3, 0), 0, 0, 5);
                std::cout << "\n6. Size 3, Rotation 90" << std::endl;
                printBoxVisualOnBoard(ConnerBox(3, 90), 0, 0, 5);
                std::cout << "\n7. Size 3, Rotation 180" << std::endl;
                printBoxVisualOnBoard(ConnerBox(3, 180), 0, 0, 5);
                std::cout << "\n8. Size 3, Rotation 270" << std::endl;
                printBoxVisualOnBoard(ConnerBox(3, 270), 0, 0, 5);
                std::cout << "Enter your choice (1-8): ";
                std::cin >> subChoice;
                Box selectedBox = SquareBox(1); // Default
                std::string desc;
                switch (subChoice) {
                    case 1: selectedBox = ConnerBox(2, 0); desc = "Size 2, Rotation 0"; break;
                    case 2: selectedBox = ConnerBox(2, 90); desc = "Size 2, Rotation 90"; break;
                    case 3: selectedBox = ConnerBox(2, 180); desc = "Size 2, Rotation 180"; break;
                    case 4: selectedBox = ConnerBox(2, 270); desc = "Size 2, Rotation 270"; break;
                    case 5: selectedBox = ConnerBox(3, 0); desc = "Size 3, Rotation 0"; break;
                    case 6: selectedBox = ConnerBox(3, 90); desc = "Size 3, Rotation 90"; break;
                    case 7: selectedBox = ConnerBox(3, 180); desc = "Size 3, Rotation 180"; break;
                    case 8: selectedBox = ConnerBox(3, 270); desc = "Size 3, Rotation 270"; break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        continue;
                }
                return selectedBox;
            }
            case 5: {
                std::cout << "========================================" << std::endl;
                std::cout << "       T-SHAPE BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select rotation for TShapeBox:" << std::endl;
                std::cout << "\n1. Rotation 0" << std::endl;
                printBoxVisualOnBoard(TShapeBox(0), 0, 0, 5);
                std::cout << "\n2. Rotation 90" << std::endl;
                printBoxVisualOnBoard(TShapeBox(90), 0, 0, 5);
                std::cout << "\n3. Rotation 180" << std::endl;
                printBoxVisualOnBoard(TShapeBox(180), 0, 0, 5);
                std::cout << "\n4. Rotation 270" << std::endl;
                printBoxVisualOnBoard(TShapeBox(270), 0, 0, 5);
                std::cout << "Enter your choice (1-4): ";
                std::cin >> subChoice;
                Box selectedBox = SquareBox(1); // Default
                std::string desc;
                switch (subChoice) {
                    case 1: selectedBox = TShapeBox(0); desc = "Rotation 0"; break;
                    case 2: selectedBox = TShapeBox(90); desc = "Rotation 90"; break;
                    case 3: selectedBox = TShapeBox(180); desc = "Rotation 180"; break;
                    case 4: selectedBox = TShapeBox(270); desc = "Rotation 270"; break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        continue;
                }
                return selectedBox;
            }
            case 6: {
                std::cout << "========================================" << std::endl;
                std::cout << "       Z-SHAPE BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select type for ZShapeBox:" << std::endl;
                std::cout << "\n1. Left Z, Horizontal (0/180)" << std::endl;
                printBoxVisualOnBoard(ZShapeBox("left", 0), 0, 0, 5);
                std::cout << "\n2. Left Z, Vertical (90/270)" << std::endl;
                printBoxVisualOnBoard(ZShapeBox("left", 90), 0, 0, 5);
                std::cout << "\n3. Right Z, Horizontal (0/180)" << std::endl;
                printBoxVisualOnBoard(ZShapeBox("right", 0), 0, 0, 5);
                std::cout << "\n4. Right Z, Vertical (90/270)" << std::endl;
                printBoxVisualOnBoard(ZShapeBox("right", 90), 0, 0, 5);
                std::cout << "Enter your choice (1-4): ";
                std::cin >> subChoice;
                Box selectedBox = SquareBox(1); // Default
                std::string desc;
                switch (subChoice) {
                    case 1: selectedBox = ZShapeBox("left", 0); desc = "Left Z, Horizontal"; break;
                    case 2: selectedBox = ZShapeBox("left", 90); desc = "Left Z, Vertical"; break;
                    case 3: selectedBox = ZShapeBox("right", 0); desc = "Right Z, Horizontal"; break;
                    case 4: selectedBox = ZShapeBox("right", 90); desc = "Right Z, Vertical"; break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        continue;
                }
                return selectedBox;
            }
            case 7: {
                std::cout << "========================================" << std::endl;
                std::cout << "       DIAGONAL BOX SELECTION" << std::endl;
                std::cout << "========================================" << std::endl;
                std::cout << "Select type for DiagonalBox:" << std::endl;
                std::cout << "\n1. Left Diagonal, Size 2" << std::endl;
                printBoxVisualOnBoard(DiagonalBox("left", 2), 0, 0, 5);
                std::cout << "\n2. Right Diagonal, Size 2" << std::endl;
                printBoxVisualOnBoard(DiagonalBox("right", 2), 0, 0, 5);
                std::cout << "\n3. Left Diagonal, Size 3" << std::endl;
                printBoxVisualOnBoard(DiagonalBox("left", 3), 0, 0, 5);
                std::cout << "\n4. Right Diagonal, Size 3" << std::endl;
                printBoxVisualOnBoard(DiagonalBox("right", 3), 0, 0, 5);
                std::cout << "Enter your choice (1-4): ";
                std::cin >> subChoice;
                Box selectedBox = SquareBox(1); // Default
                std::string desc;
                switch (subChoice) {
                    case 1: selectedBox = DiagonalBox("left", 2); desc = "Left Diagonal, Size 2"; break;
                    case 2: selectedBox = DiagonalBox("right", 2); desc = "Right Diagonal, Size 2"; break;
                    case 3: selectedBox = DiagonalBox("left", 3); desc = "Left Diagonal, Size 3"; break;
                    case 4: selectedBox = DiagonalBox("right", 3); desc = "Right Diagonal, Size 3"; break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        continue;
                }
                return selectedBox;
            }
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 7." << std::endl;
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
                std::cout << "\033[33m[ Duration: " << durationMicroSecond << " μs ]\033[0m" << std::endl;
            };
            
            // Phase 1: Validate all cells before placing any
            for (const auto& coord : box.getBody()) {
                int x = startX + coord.getX();
                int y = startY + coord.getY();
                if (!board.isInBounds(x, y) || board.isCellFilled(x, y)) {
                    std::cout << "\033[31mCannot place box " << box.getName() << " at (" << startX << ", " << startY << ").\033[0m" << std::endl;
                    std::cout << "\033[36mRollback any changes made.\033[0m" << std::endl;
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
            std::cout << "\033[32mSuccessfully placed box " << box.getName() << " at (" << startX << ", " << startY << ").\033[0m" << std::endl;
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
                std::cout << "\033[33m[ Duration: " << duration.count() << " μs ]\033[0m" << std::endl;
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
                        
                        std::cout << "\033[32m[BruteForce] Successfully placed " << box.getName() << " at (" << row << ", " << col << ").\033[0m" << std::endl;
                        printDuration();
                        
                        // Print board with placed cells highlighted in red
                        std::cout << "\n[Result Board - Red = Newly Placed]" << std::endl;
                        printBoardWithHighlight(tempBoard, placedCoords);
                        
                        return tempBoard;
                    }
                }
            }
            std::cout << "\033[31m[BruteForce] Could not place " << box.getName() << " on the board.\033[0m" << std::endl;
            printDuration();
            return board; // Return original board if no placement found
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
            std::cout << "\n\033[36m========== BLAST THE BOARD RESULTS ==========\033[0m\n";
            
            if (fullRows.empty() && fullCols.empty()) {
                std::cout << "\033[33m⚠ No full rows or columns detected.\033[0m\n";
            } else {
                // Row clearing info
                if (!fullRows.empty()) {
                    std::cout << "\033[32m✓ Rows Cleared: " << info.clearedRows << "\033[0m";
                    std::cout << " → [";
                    for (size_t i = 0; i < fullRows.size(); ++i) {
                        std::cout << "Row " << fullRows[i];
                        if (i < fullRows.size() - 1) std::cout << ", ";
                    }
                    std::cout << "]\n";
                }
                
                // Column clearing info
                if (!fullCols.empty()) {
                    std::cout << "\033[32m✓ Columns Cleared: " << info.clearedCols << "\033[0m";
                    std::cout << " → [";
                    for (size_t i = 0; i < fullCols.size(); ++i) {
                        std::cout << "Col " << fullCols[i];
                        if (i < fullCols.size() - 1) std::cout << ", ";
                    }
                    std::cout << "]\n";
                }
                
                // Combo info (intersections)
                if (comboCount > 0) {
                    std::cout << "\033[35m★ COMBO x" << comboCount << "!\033[0m";
                    std::cout << " (Row+Column intersections at: ";
                    for (size_t i = 0; i < comboIntersections.size(); ++i) {
                        std::cout << "(" << comboIntersections[i].first << "," << comboIntersections[i].second << ")";
                        if (i < comboIntersections.size() - 1) std::cout << ", ";
                    }
                    std::cout << ")\n";
                }
                
                // Total cells cleared
                std::cout << "\033[34m◆ Total Cells Cleared: " << info.totalClearedCells << "\033[0m";
                
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
            
            std::cout << "\033[90m⏱ Duration: " << duration.count() << " μs\033[0m\n";
            std::cout << "\033[36m==============================================\033[0m\n\n";
            
            return info;
        }

    private:
        // Helper method to print board with specific cells highlighted in red
        void printBoardWithHighlight(const Board& boardToPrint, const std::vector<std::pair<int, int>>& highlightCoords) const {
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
                        // Red color for newly placed cells
                        std::cout << "[\033[31m#\033[0m]";
                    } else if (boardToPrint.isCellFilled(row, col)) {
                        // Normal color for existing filled cells
                        std::cout << "[#]";
                    } else {
                        // Empty cell
                        std::cout << "[ ]";
                    }
                }
                std::cout << std::endl;
            }
        }
};

Box randomShapeInBundle () {
    std::vector<Box> boxes = BoxBundle();
    int randomIndex = rand() % boxes.size();
    return boxes[randomIndex];
}


int main() {
    srand(time(NULL));

    Board tempBoard;
    
    BlockBlastGame game;
    game.displayBoard();
    game.getEmptyCellCount();

    for (int i = 0; i <= 20; ++i) {
        Box selectedBox = randomShapeInBundle();
        // NotifySelectedBox(selectedBox);
        
        tempBoard = game.BruteForceAlgorithm(selectedBox);
        game.useThisBoard(tempBoard);
        game.displayBoard();
        game.getEmptyCellCount();
        
        // Check and clear any full rows/columns
        ClearedBoardInfo clearInfo = game.blastTheBoard();
        
        // Show the board after blasting if anything was cleared
        if (clearInfo.clearedRows > 0 || clearInfo.clearedCols > 0) {
            std::cout << "\n[Board After Blasting]" << std::endl;
            game.displayBoard();
            game.getEmptyCellCount();
        }
    }
    
    // while (true) {
    //     Box selectedBox = MenuBoxSelection();
    //     NotifySelectedBox(selectedBox);
        
    //     tempBoard = game.BruteForceAlgorithm(selectedBox);
    //     game.useThisBoard(tempBoard);
    //     game.displayBoard();
    //     game.getEmptyCellCount();
        
    //     // Check and clear any full rows/columns
    //     ClearedBoardInfo clearInfo = game.blastTheBoard();
        
    //     // Show the board after blasting if anything was cleared
    //     if (clearInfo.clearedRows > 0 || clearInfo.clearedCols > 0) {
    //         std::cout << "\n[Board After Blasting]" << std::endl;
    //         game.displayBoard();
    //         game.getEmptyCellCount();
    //     }
        
    //     char continueChoice;
    //     std::cout << "Do you want to place another box? (y/n): ";
    //     std::cin >> continueChoice;
    //     if (continueChoice != 'y' && continueChoice != 'Y') {
    //         break;
    //     }
    // }
    

    // game.randomlyPlaceBlocks();
    // game.displayBoard();
    // game.getEmptyCellCount();

    // game.placeBoxOnBoard(SquareBox(3), 6, 0);
    // std::cout << "\nAfter placing a 3x3 SquareBox at (0,0):\n";
    // game.displayBoard();
    // game.getEmptyCellCount();

    // Board simBoard = game.BruteForceAlgorithm(RectangleBox(2,3));
    // simBoard.printVisual();
    // std::cout << "\nAfter BruteForceAlgorithm to place a 2x3 RectangleBox:\n";
    // game.displayBoard();
    // game.getEmptyCellCount();

    // printf("Box Visual Representation:\n");
    // std::vector<Box> boxes = BoxBundle();
    // for (const auto& box : boxes) {
    //     std::cout << "Box Name: " << box.getName() << std::endl;
    //     printBoxVisualOnBoard(box, 0, 0, 5);
    //     std::cout << std::endl;
    // }

    // Box selectedBox = MenuBoxSelection();
    // NotifySelectedBox(selectedBox);
    
    return 0;
}