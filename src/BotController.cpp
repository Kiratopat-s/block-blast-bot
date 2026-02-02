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
    std::cout << "\nCurrent Board:" << std::endl;
    std::cout << state_.getBoard().toString() << std::endl;
}

void BotController::displayPieces() const {
    std::cout << "\nAvailable Pieces:" << std::endl;
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (!state_.isPieceUsed(i)) {
            std::cout << "Piece " << (i + 1) << ":" << std::endl;
            std::cout << state_.getPiece(i).toString() << std::endl;
        }
    }
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

std::array<Piece, PIECES_PER_TURN> BotController::readPiecesFromInput() {
    std::array<Piece, PIECES_PER_TURN> pieces;
    
    std::cout << "Enter piece type for each (1-13 for predefined, 0 for random):" << std::endl;
    std::cout << "1=Single, 2=Dot2, 3=Dot3, 4=Dot4, 5=Dot5, 6=Square2, 7=Square3" << std::endl;
    std::cout << "8=L_Small, 9=L_Large, 10=T, 11=Z, 12=Plus, 13=Corner3" << std::endl;
    
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        int type;
        std::cout << "Piece " << (i + 1) << ": ";
        std::cin >> type;
        
        if (type == 0) {
            pieces[i] = PieceFactory::createRandomPiece();
        } else if (type >= 1 && type <= 13) {
            pieces[i] = PieceFactory::createPiece(static_cast<Piece::Shape>(type - 1));
        } else {
            std::cout << "Invalid type, using random piece." << std::endl;
            pieces[i] = PieceFactory::createRandomPiece();
        }
    }
    
    return pieces;
}

void BotController::printMoveSequence(const MoveSequence& sequence) const {
    std::cout << "\n=== Best Move Sequence ===" << std::endl;
    std::cout << "Total score: " << sequence.totalScore << std::endl;
    std::cout << "Pieces placed: " << sequence.piecesPlaced << std::endl;
    
    for (int i = 0; i < sequence.piecesPlaced; ++i) {
        const auto& move = sequence.moves[i];
        std::cout << "\nMove " << (i + 1) << ":" << std::endl;
        std::cout << "  Piece: " << (move.pieceIndex + 1) << std::endl;
        std::cout << "  Position: (" << move.position.x << ", " << move.position.y << ")" << std::endl;
        std::cout << "  Rotation: " << move.rotation << std::endl;
        std::cout << "  Score: " << move.score << std::endl;
    }
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
