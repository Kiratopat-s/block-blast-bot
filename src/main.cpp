#include "BotController.h"
#include <iostream>

using namespace BlockBlast;

int main(int argc, char* argv[]) {
    // Parse command-line arguments
    auto options = CLI::parseArgs(argc, argv);
    
    // Create solver configuration
    Solver::Config solverConfig = options.solverConfig;
    solverConfig.weights = options.weights;
    
    // Create bot controller
    BotController bot(solverConfig);
    
    try {
        if (options.interactive) {
            // Run interactive mode
            bot.runInteractive();
        } else {
            // Run automatic mode
            bot.runAutomatic(options.numGames);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
