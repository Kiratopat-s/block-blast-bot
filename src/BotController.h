#pragma once

#include "../include/common.h"
#include "../src/core/GameState.h"
#include "../src/ai/Solver.h"
#include <string>
#include <vector>

namespace BlockBlast {

/**
 * Main bot controller that manages the game loop and user interaction.
 */
class BotController {
public:
    BotController();
    explicit BotController(const Solver::Config& config);
    
    // Main control methods
    void run();
    void runInteractive();
    void runAutomatic(int numGames = 1);
    
    // Input/Output
    void displayBoard() const;
    void displayPieces() const;
    void displayStats() const;
    void printMoveSequence(const MoveSequence& sequence) const;
    
    // Game control
    void newGame();
    void setPieces(const std::array<Piece, PIECES_PER_TURN>& pieces);
    MoveSequence computeBestMoves();
    bool executeSequence(const MoveSequence& sequence);
    
    // Configuration
    void setSolverConfig(const Solver::Config& config);
    void setWeights(const ScoringWeights& weights);
    
    // State access
    const GameState& getState() const { return state_; }
    GameState& getState() { return state_; }
    
private:
    GameState state_;
    Solver solver_;
    
    // Helper methods
    bool readBoardFromInput();
    std::array<Piece, PIECES_PER_TURN> readPiecesFromInput();
    Piece parsePieceFromString(const std::string& str);
    void displayPieceGallery() const;
    void displayPieceWithRotations(Piece::Shape shape, int number) const;
    Piece selectPieceInteractive(int pieceNumber);
};

/**
 * Command-line interface parser
 */
class CLI {
public:
    struct Options {
        bool interactive = true;
        bool verbose = true;
        int numGames = 1;
        std::string configFile;
        ScoringWeights weights;
        Solver::Config solverConfig;
    };
    
    static Options parseArgs(int argc, char* argv[]);
    static void printHelp();
    static void printVersion();
};

} // namespace BlockBlast
