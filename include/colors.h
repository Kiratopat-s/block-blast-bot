#pragma once

#include <string>

namespace BlockBlast {

/**
 * ANSI color codes for terminal output
 */
namespace Colors {
    // Reset
    const std::string RESET = "\033[0m";
    
    // Regular colors
    const std::string BLACK = "\033[30m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    
    // Bold colors
    const std::string BOLD_BLACK = "\033[1;30m";
    const std::string BOLD_RED = "\033[1;31m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_YELLOW = "\033[1;33m";
    const std::string BOLD_BLUE = "\033[1;34m";
    const std::string BOLD_MAGENTA = "\033[1;35m";
    const std::string BOLD_CYAN = "\033[1;36m";
    const std::string BOLD_WHITE = "\033[1;37m";
    
    // Background colors
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
    
    // Bright background colors
    const std::string BG_BRIGHT_BLACK = "\033[100m";
    const std::string BG_BRIGHT_RED = "\033[101m";
    const std::string BG_BRIGHT_GREEN = "\033[102m";
    const std::string BG_BRIGHT_YELLOW = "\033[103m";
    const std::string BG_BRIGHT_BLUE = "\033[104m";
    const std::string BG_BRIGHT_MAGENTA = "\033[105m";
    const std::string BG_BRIGHT_CYAN = "\033[106m";
    const std::string BG_BRIGHT_WHITE = "\033[107m";
    
    // Special effects
    const std::string BOLD = "\033[1m";
    const std::string DIM = "\033[2m";
    const std::string UNDERLINE = "\033[4m";
    const std::string BLINK = "\033[5m";
    const std::string REVERSE = "\033[7m";
    
    // Piece placement colors (vibrant and distinct)
    const std::string PIECE_1 = BOLD_CYAN;     // First piece - Cyan
    const std::string PIECE_2 = BOLD_YELLOW;   // Second piece - Yellow
    const std::string PIECE_3 = BOLD_MAGENTA;  // Third piece - Magenta
    const std::string CLEARED_LINE = BOLD_GREEN;  // Cleared lines - Green
    const std::string EXISTING = WHITE;        // Existing blocks - White
    const std::string BOARD_BORDER = BOLD_BLUE;   // Board borders - Blue
    
    // Get piece color by index
    inline std::string getPieceColor(int index) {
        switch (index) {
            case 0: return PIECE_1;
            case 1: return PIECE_2;
            case 2: return PIECE_3;
            default: return WHITE;
        }
    }
}

} // namespace BlockBlast
