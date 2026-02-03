#include "BotController.h"
#include <iostream>
#include <sstream>
#include <iomanip>

namespace BlockBlast {

BotController::BotController() 
    : solver_() {}

BotController::BotController(const Solver::Config& config) 
    : solver_(config) {}

void BotController::run() {
    runInteractive();
}

void BotController::runInteractive() {
    std::cout << "=== Block Blast Bot ===" << std::endl;
    std::cout << "Enter board state and pieces to get optimal moves" << std::endl;
    
    while (true) {
        std::cout << "\nOptions:" << std::endl;
        std::cout << "1. Enter new board state" << std::endl;
        std::cout << "2. Use empty board" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 3) break;
        
        if (choice == 1) {
            if (!readBoardFromInput()) {
                std::cout << "Invalid board input!" << std::endl;
                continue;
            }
        } else if (choice == 2) {
            newGame();
        } else {
            std::cout << "Invalid choice!" << std::endl;
            continue;
        }
        
        // Read pieces
        std::cout << "\nEnter 3 pieces:" << std::endl;
        auto pieces = readPiecesFromInput();
        setPieces(pieces);
        
        // Display current state
        displayBoard();
        displayPieces();
        
        // Compute best moves
        std::cout << "\nComputing best moves..." << std::endl;
        auto sequence = computeBestMoves();
        
        // Display results
        printMoveSequence(sequence);
        displayStats();
    }
}

void BotController::runAutomatic(int numGames) {
    std::cout << "Running " << numGames << " automatic games..." << std::endl;
    
    int totalScore = 0;
    int gamesCompleted = 0;
    
    for (int game = 0; game < numGames; ++game) {
        newGame();
        
        bool gameOver = false;
        int turnCount = 0;
        
        while (!gameOver && turnCount < 100) {
            // Generate random pieces
            auto pieces = PieceFactory::createRandomSet();
            setPieces(pieces);
            
            // Compute and execute best sequence
            auto sequence = computeBestMoves();
            
            if (sequence.piecesPlaced == 0) {
                gameOver = true;
                break;
            }
            
            executeSequence(sequence);
            turnCount++;
            
            // Check if game over
            if (state_.isGameOver()) {
                gameOver = true;
            }
        }
        
        totalScore += state_.getScore();
        gamesCompleted++;
        
        std::cout << "Game " << (game + 1) << " completed: Score = " 
                  << state_.getScore() << ", Turns = " << turnCount << std::endl;
    }
    
    if (gamesCompleted > 0) {
        std::cout << "\nAverage score: " << (totalScore / gamesCompleted) << std::endl;
    }
}

void BotController::displayBoard() const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  CURRENT BOARD (8x8)" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << state_.getBoard().toString();
    std::cout << "  Score: " << state_.getScore() << " | Combo: " << state_.getComboCount() << "x" << std::endl;
}

void BotController::displayPieces() const {
    std::cout << "\n" << std::string(50, '-') << std::endl;
    std::cout << "  AVAILABLE PIECES" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    int displayCount = 0;
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (!state_.isPieceUsed(i)) {
            std::cout << "\n  Piece #" << (i + 1) << ":" << std::endl;
            std::cout << state_.getPiece(i).toString();
            displayCount++;
        }
    }
    
    if (displayCount == 0) {
        std::cout << "  No pieces available (all used)" << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
}

void BotController::displayStats() const {
    const auto& stats = solver_.getStats();
    
    std::cout << "\n=== Statistics ===" << std::endl;
    std::cout << "Nodes evaluated: " << stats.nodesEvaluated << std::endl;
    std::cout << "Nodes generated: " << stats.nodesGenerated << std::endl;
    std::cout << "Time: " << stats.timeMs << " ms" << std::endl;
    std::cout << "Best score: " << std::fixed << std::setprecision(2) 
              << stats.bestScore << std::endl;
}

void BotController::newGame() {
    state_.reset();
}

void BotController::setPieces(const std::array<Piece, PIECES_PER_TURN>& pieces) {
    state_.setPieces(pieces);
}

MoveSequence BotController::computeBestMoves() {
    return solver_.findBestSequence(state_);
}

bool BotController::executeSequence(const MoveSequence& sequence) {
    bool allSuccess = true;
    
    for (int i = 0; i < sequence.piecesPlaced; ++i) {
        if (!state_.executeMove(sequence.moves[i])) {
            allSuccess = false;
            break;
        }
    }
    
    return allSuccess;
}

void BotController::setSolverConfig(const Solver::Config& config) {
    solver_.setConfig(config);
}

void BotController::setWeights(const ScoringWeights& weights) {
    auto config = solver_.getConfig();
    config.weights = weights;
    solver_.setConfig(config);
}

bool BotController::readBoardFromInput() {
    std::cout << "Enter board state (8 lines of 8 characters, '.' for empty, 'X' for occupied):" << std::endl;
    
    state_.getBoard().clear();
    
    for (int y = 0; y < BOARD_SIZE; ++y) {
        std::string line;
        std::cin >> line;
        
        if (line.length() != BOARD_SIZE) {
            return false;
        }
        
        for (int x = 0; x < BOARD_SIZE; ++x) {
            if (line[x] == 'X' || line[x] == 'x' || line[x] == '1') {
                state_.getBoard().setCell(x, y, true);
            }
        }
    }
    
    return true;
}

void BotController::displayPieceGallery() const {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "  PIECE GALLERY - Select from available shapes" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    // Display pieces in a grid layout
    const std::vector<std::pair<Piece::Shape, std::string>> pieceList = {
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
    
    // Display 3 pieces per row
    for (size_t i = 0; i < pieceList.size(); i += 3) {
        // Print numbers
        std::cout << "  ";
        for (size_t j = i; j < std::min(i + 3, pieceList.size()); ++j) {
            std::cout << "[" << (j + 1) << "] " << std::setw(12) << std::left << pieceList[j].second;
            if (j < std::min(i + 3, pieceList.size()) - 1) std::cout << "  ";
        }
        std::cout << std::endl;
        
        // Get max height for this row
        int maxHeight = 0;
        for (size_t j = i; j < std::min(i + 3, pieceList.size()); ++j) {
            auto piece = PieceFactory::createPiece(pieceList[j].first);
            maxHeight = std::max(maxHeight, piece.getHeight());
        }
        
        // Print pieces row by row
        for (int row = 0; row < maxHeight; ++row) {
            std::cout << "  ";
            for (size_t j = i; j < std::min(i + 3, pieceList.size()); ++j) {
                auto piece = PieceFactory::createPiece(pieceList[j].first);
                std::cout << "  ";
                
                if (row < piece.getHeight()) {
                    for (int col = 0; col < piece.getWidth(); ++col) {
                        bool found = false;
                        for (const auto& cell : piece.getCells()) {
                            if (cell.x == col && cell.y == row) {
                                found = true;
                                break;
                            }
                        }
                        std::cout << (found ? "██" : "  ");
                    }
                    // Padding
                    std::cout << std::string(12 - piece.getWidth() * 2, ' ');
                } else {
                    std::cout << std::string(12, ' ');
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
    std::cout << "  [0] Random piece" << std::endl;
    std::cout << "  [R] Show rotations for a piece" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
}

void BotController::displayPieceWithRotations(Piece::Shape shape, int number) const {
    auto basePiece = PieceFactory::createPiece(shape);
    auto rotations = basePiece.getAllRotations();
    
    std::cout << "\n" << std::string(70, '-') << std::endl;
    std::cout << "  PIECE #" << number << " - Rotation Patterns (" << rotations.size() << " unique)" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (size_t i = 0; i < rotations.size(); ++i) {
        std::cout << "\n  Rotation " << i << " (" << (i * 90) << "°):" << std::endl;
        const auto& piece = rotations[i];
        
        for (int row = 0; row < piece.getHeight(); ++row) {
            std::cout << "    ";
            for (int col = 0; col < piece.getWidth(); ++col) {
                bool found = false;
                for (const auto& cell : piece.getCells()) {
                    if (cell.x == col && cell.y == row) {
                        found = true;
                        break;
                    }
                }
                std::cout << (found ? "██" : "  ");
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::string(70, '-') << std::endl;
}

Piece BotController::selectPieceInteractive(int pieceNumber) {
    while (true) {
        displayPieceGallery();
        
        std::cout << "\nSelect piece #" << pieceNumber << " (1-13, 0 for random, R to see rotations): ";
        std::string input;
        std::cin >> input;
        
        // Check if user wants to see rotations
        if (input == "R" || input == "r") {
            std::cout << "Enter piece number to view rotations (1-13): ";
            int pieceNum;
            std::cin >> pieceNum;
            if (pieceNum >= 1 && pieceNum <= 13) {
                displayPieceWithRotations(static_cast<Piece::Shape>(pieceNum - 1), pieceNum);
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
            continue;
        }
        
        // Try to parse as number
        try {
            int type = std::stoi(input);
            
            if (type == 0) {
                std::cout << "  ✓ Random piece selected" << std::endl;
                return PieceFactory::createRandomPiece();
            } else if (type >= 1 && type <= 13) {
                auto piece = PieceFactory::createPiece(static_cast<Piece::Shape>(type - 1));
                std::cout << "  ✓ Piece selected" << std::endl;
                return piece;
            } else {
                std::cout << "  ✗ Invalid number. Please enter 0-13." << std::endl;
            }
        } catch (...) {
            std::cout << "  ✗ Invalid input. Please enter a number (0-13) or R." << std::endl;
        }
    }
}

std::array<Piece, PIECES_PER_TURN> BotController::readPiecesFromInput() {
    std::array<Piece, PIECES_PER_TURN> pieces;
    
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "  SELECT 3 PIECES" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        pieces[i] = selectPieceInteractive(i + 1);
    }
    
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << "  ✓ All pieces selected!" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    
    return pieces;
}

void BotController::printMoveSequence(const MoveSequence& sequence) const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  RECOMMENDED MOVES" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "  Total Score: " << std::fixed << std::setprecision(1) << sequence.totalScore << std::endl;
    std::cout << "  Pieces Placed: " << sequence.piecesPlaced << "/" << PIECES_PER_TURN << std::endl;
    
    if (sequence.piecesPlaced == 0) {
        std::cout << "\n  ⚠ No valid moves found! Game Over." << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        return;
    }
    
    // Create a preview board for showing placements
    GameState previewState = state_;
    
    for (int i = 0; i < sequence.piecesPlaced; ++i) {
        const auto& move = sequence.moves[i];
        std::cout << "\n  ┌" << std::string(44, '-') << "┐" << std::endl;
        std::cout << "  │ Move #" << (i + 1) << ": Piece #" << (move.pieceIndex + 1) 
                  << std::string(28 - std::to_string(i+1).length() - std::to_string(move.pieceIndex+1).length(), ' ') << "│" << std::endl;
        std::cout << "  ├" << std::string(44, '-') << "┤" << std::endl;
        std::cout << "  │ Place at: (" << move.position.x << ", " << move.position.y << ")"
                  << std::string(30 - std::to_string(move.position.x).length() - std::to_string(move.position.y).length(), ' ') << "│" << std::endl;
        std::cout << "  │ Rotation: " << move.rotation 
                  << std::string(31 - std::to_string(move.rotation).length(), ' ') << "│" << std::endl;
        std::cout << "  │ Score Impact: +" << std::fixed << std::setprecision(1) << move.score
                  << std::string(27 - std::to_string(static_cast<int>(move.score)).length(), ' ') << "│" << std::endl;
        std::cout << "  └" << std::string(44, '-') << "┘" << std::endl;
        
        // Execute move on preview for visual
        previewState.executeMove(move);
    }
    
    // Show final board state after all moves
    std::cout << "\n  Final Board After Moves:" << std::endl;
    std::cout << previewState.getBoard().toString();
    std::cout << "  Final Score: " << previewState.getScore() 
              << " | Combo: " << previewState.getComboCount() << "x" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

// CLI implementations
CLI::Options CLI::parseArgs(int argc, char* argv[]) {
    Options opts;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printHelp();
            exit(0);
        } else if (arg == "-v" || arg == "--version") {
            printVersion();
            exit(0);
        } else if (arg == "-a" || arg == "--auto") {
            opts.interactive = false;
            if (i + 1 < argc) {
                opts.numGames = std::stoi(argv[++i]);
            }
        } else if (arg == "-q" || arg == "--quiet") {
            opts.verbose = false;
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                opts.configFile = argv[++i];
            }
        } else if (arg == "--beam-width") {
            if (i + 1 < argc) {
                opts.solverConfig.beamWidth = std::stoi(argv[++i]);
            }
        } else if (arg == "--max-depth") {
            if (i + 1 < argc) {
                opts.solverConfig.maxDepth = std::stoi(argv[++i]);
            }
        } else if (arg == "--threads") {
            if (i + 1 < argc) {
                opts.solverConfig.numThreads = std::stoi(argv[++i]);
            }
        } else if (arg == "--no-parallel") {
            opts.solverConfig.useParallel = false;
        }
    }
    
    return opts;
}

void CLI::printHelp() {
    std::cout << "Block Blast Bot - AI solver for Block Blast game\n" << std::endl;
    std::cout << "Usage: block-blast-bot [OPTIONS]\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h, --help              Show this help message" << std::endl;
    std::cout << "  -v, --version           Show version information" << std::endl;
    std::cout << "  -a, --auto <N>          Run N automatic games" << std::endl;
    std::cout << "  -q, --quiet             Quiet mode (less output)" << std::endl;
    std::cout << "  -c, --config <file>     Load configuration from file" << std::endl;
    std::cout << "  --beam-width <N>        Set beam search width (default: 50)" << std::endl;
    std::cout << "  --max-depth <N>         Set maximum search depth (default: 3)" << std::endl;
    std::cout << "  --threads <N>           Set number of threads (default: auto)" << std::endl;
    std::cout << "  --no-parallel           Disable parallel processing" << std::endl;
}

void CLI::printVersion() {
    std::cout << "Block Blast Bot v1.0.0" << std::endl;
    std::cout << "High-performance AI solver for 8x8 Block Blast" << std::endl;
}

} // namespace BlockBlast
