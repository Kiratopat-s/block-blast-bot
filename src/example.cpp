#include "BotController.h"
#include "core/Piece.h"
#include <iostream>

using namespace BlockBlast;

void runExample() {
    std::cout << "=== Block Blast Bot Example ===" << std::endl;
    
    // Create bot with default configuration
    BotController bot;
    
    // Start with empty board
    bot.newGame();
    
    // Create 3 example pieces
    std::array<Piece, PIECES_PER_TURN> pieces = {
        PieceFactory::createPiece(Piece::Shape::SQUARE_2),  // 2x2 square
        PieceFactory::createPiece(Piece::Shape::DOT_3),     // 3 horizontal
        PieceFactory::createPiece(Piece::Shape::L_SMALL)    // Small L
    };
    
    bot.setPieces(pieces);
    
    std::cout << "\n--- Initial State ---" << std::endl;
    bot.displayBoard();
    bot.displayPieces();
    
    // Compute best move sequence
    std::cout << "\n--- Computing Best Moves ---" << std::endl;
    auto sequence = bot.computeBestMoves();
    
    std::cout << "\nFound sequence with " << sequence.piecesPlaced 
              << " pieces placed" << std::endl;
    std::cout << "Total score: " << sequence.totalScore << std::endl;
    
    // Execute the sequence
    bot.executeSequence(sequence);
    
    std::cout << "\n--- Final State ---" << std::endl;
    bot.displayBoard();
    bot.displayStats();
}

void runBenchmark() {
    std::cout << "\n=== Performance Benchmark ===" << std::endl;
    
    #ifdef _OPENMP
    std::cout << "OpenMP enabled - Running parallel and serial comparisons" << std::endl;
    #else
    std::cout << "OpenMP not available - Running serial only" << std::endl;
    #endif
    
    Solver::Config config;
    config.beamWidth = 50;
    config.maxDepth = 3;
    
    const int numTrials = 10;
    
    #ifdef _OPENMP
    // Test with parallelization
    std::cout << "\n-- With Parallelization --" << std::endl;
    config.useParallel = true;
    BotController parallelBot(config);
    
    int parallelTotalTime = 0;
    for (int i = 0; i < numTrials; ++i) {
        parallelBot.newGame();
        auto pieces = PieceFactory::createRandomSet();
        parallelBot.setPieces(pieces);
        
        auto start = std::chrono::high_resolution_clock::now();
        auto sequence = parallelBot.computeBestMoves();
        auto end = std::chrono::high_resolution_clock::now();
        
        int timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start).count();
        parallelTotalTime += timeMs;
        
        std::cout << "Trial " << (i+1) << ": " << timeMs << " ms, "
                  << "pieces placed: " << sequence.piecesPlaced << std::endl;
    }
    std::cout << "Average time (parallel): " << (parallelTotalTime / numTrials) << " ms" << std::endl;
    
    // Test without parallelization
    std::cout << "\n-- Without Parallelization --" << std::endl;
    config.useParallel = false;
    BotController serialBot(config);
    
    int serialTotalTime = 0;
    for (int i = 0; i < numTrials; ++i) {
        serialBot.newGame();
        auto pieces = PieceFactory::createRandomSet();
        serialBot.setPieces(pieces);
        
        auto start = std::chrono::high_resolution_clock::now();
        auto sequence = serialBot.computeBestMoves();
        auto end = std::chrono::high_resolution_clock::now();
        
        int timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start).count();
        serialTotalTime += timeMs;
        
        std::cout << "Trial " << (i+1) << ": " << timeMs << " ms, "
                  << "pieces placed: " << sequence.piecesPlaced << std::endl;
    }
    std::cout << "Average time (serial): " << (serialTotalTime / numTrials) << " ms" << std::endl;
    
    float speedup = static_cast<float>(serialTotalTime) / parallelTotalTime;
    std::cout << "\nSpeedup: " << std::fixed << std::setprecision(2) 
              << speedup << "x" << std::endl;
    #else
    // Serial only
    BotController bot(config);
    int totalTime = 0;
    
    for (int i = 0; i < numTrials; ++i) {
        bot.newGame();
        auto pieces = PieceFactory::createRandomSet();
        bot.setPieces(pieces);
        
        auto start = std::chrono::high_resolution_clock::now();
        auto sequence = bot.computeBestMoves();
        auto end = std::chrono::high_resolution_clock::now();
        
        int timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            end - start).count();
        totalTime += timeMs;
        
        std::cout << "Trial " << (i+1) << ": " << timeMs << " ms, "
                  << "pieces placed: " << sequence.piecesPlaced << std::endl;
    }
    
    std::cout << "\nAverage time: " << (totalTime / numTrials) << " ms" << std::endl;
    #endif
}

void compareAlgorithms() {
    std::cout << "\n=== Algorithm Comparison ===" << std::endl;
    
    GameState state;
    auto pieces = PieceFactory::createRandomSet();
    state.setPieces(pieces);
    
    // Test greedy solver
    std::cout << "\n-- Greedy Solver --" << std::endl;
    GreedySolver greedySolver;
    auto start = std::chrono::high_resolution_clock::now();
    auto greedyResult = greedySolver.solve(state);
    auto end = std::chrono::high_resolution_clock::now();
    auto greedyTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start).count();
    
    std::cout << "Time: " << greedyTime << " ms" << std::endl;
    std::cout << "Pieces placed: " << greedyResult.piecesPlaced << std::endl;
    std::cout << "Score: " << greedyResult.totalScore << std::endl;
    
    // Test beam search solver
    std::cout << "\n-- Beam Search Solver --" << std::endl;
    state.reset();
    state.setPieces(pieces);
    
    Solver solver;
    start = std::chrono::high_resolution_clock::now();
    auto beamResult = solver.findBestSequence(state);
    end = std::chrono::high_resolution_clock::now();
    auto beamTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start).count();
    
    std::cout << "Time: " << beamTime << " ms" << std::endl;
    std::cout << "Pieces placed: " << beamResult.piecesPlaced << std::endl;
    std::cout << "Score: " << beamResult.totalScore << std::endl;
    std::cout << "Nodes evaluated: " << solver.getStats().nodesEvaluated << std::endl;
    
    std::cout << "\nBeam search score improvement: " 
              << ((beamResult.totalScore - greedyResult.totalScore) / greedyResult.totalScore * 100.0f)
              << "%" << std::endl;
}

int main() {
    try {
        runExample();
        runBenchmark();
        compareAlgorithms();
        
        std::cout << "\n=== Example Complete ===" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
