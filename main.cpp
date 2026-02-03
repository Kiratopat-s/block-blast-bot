#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

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

class BlockBlastGame {
    private:
        static const int BOARD_SIZE = 8;
        static const char FILLED_CELL = '#';
        static const char EMPTY_CELL = ' ';
        
        bool board[BOARD_SIZE][BOARD_SIZE];
        
        void initializeBoard() {
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    board[row][col] = false;
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
        
        void printBoardNumeric() const {
            printColumnHeaders();
            for (int row = 0; row < BOARD_SIZE; ++row) {
                std::cout << " " << row << " ";
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    std::cout << "[" << board[row][col] << "]";
                }
                std::cout << std::endl;
            }
        }
        
        void printBoardVisual() const {
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

    public:
        BlockBlastGame() {
            initializeBoard();
        }
        
        void getEmptyCells() const {
            std::cout << "Empty Cells (row, col):" << std::endl;
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    if (!board[row][col]) {
                        std::cout << "(" << row << ", " << col << ")" << std::endl;
                    }
                }
            }
        }

        void getEmptyCellCount() const {
            int count = 0;
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    if (!board[row][col]) {
                        ++count;
                    }
                }
            }
            std::cout << "Total Empty Cells: " << count << std::endl;
        }

        void displayBoard(bool visualMode = true) const {
            if (visualMode) {
                printBoardVisual();
            } else {
                printBoardNumeric();
            }
        }
        
        void randomlyPlaceBlocks() {
            for (int row = 0; row < BOARD_SIZE; ++row) {
                for (int col = 0; col < BOARD_SIZE; ++col) {
                    board[row][col] = (rand() % 2 == 1);
                }
            }
        }
        
        void start() {
            randomlyPlaceBlocks();
            displayBoard(true);
        }
};

int main() {
    srand(time(NULL));
    
    BlockBlastGame game;
    game.displayBoard();
    game.getEmptyCellCount();
    // game.randomlyPlaceBlocks();
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