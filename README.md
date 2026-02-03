# Block Blast Bot

A high-performance C++ AI bot for solving 8x8 Block Blast puzzles. The bot uses advanced search algorithms to find optimal piece placements considering empty space score, combo potential, and survival rate.

## Features

- **Visual Interactive Interface**: Beautiful piece gallery with rotation pattern viewer
- **Multicore/Multithreaded**: OpenMP support for 2-5x speedup on multi-core CPUs
- **Efficient Board Representation**: Uses bitset for O(1) operations
- **Advanced AI Solver**: Beam search with configurable depth and pruning
- **Multi-Criteria Evaluation**:
  - Empty space optimization
  - Combo detection and scoring
  - Survival rate calculation (future move possibilities)
  - Hole detection and height variance penalties
- **Performance Optimized**:
  - Bitboard operations
  - Smart pruning strategies
  - Configurable beam width
- **Modular Design**: Clean separation of concerns (Core, Game Logic, AI)
- **Flexible Interface**: Interactive and automatic modes

## Architecture

```
block-blast-bot/
├── include/
│   └── common.h              # Common types and constants
├── src/
│   ├── core/                 # Core game components
│   │   ├── Board.h/cpp       # Board state management
│   │   ├── Piece.h/cpp       # Piece definitions
│   │   └── GameState.h/cpp   # Game state tracking
│   ├── game/                 # Game logic
│   │   └── Evaluator.h/cpp   # Position evaluation
│   ├── ai/                   # AI solver
│   │   └── Solver.h/cpp      # Beam search solver
│   ├── BotController.h/cpp   # Main controller
│   └── main.cpp              # Entry point
├── tests/                    # Unit tests
└── CMakeLists.txt           # Build configuration
```

## Building

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15 or higher
- Optional: OpenMP for parallel processing

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure (Release mode for best performance)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
cmake --build .

# Optional: Run tests
ctest
```

## Usage

### Interactive Mode (Default)

```bash
./block-blast-bot
```

Then:

1. **Choose a board**: Empty board or enter custom 8x8 state
2. **Select 3 pieces** using the visual gallery:
   - See all 13 piece shapes displayed visually
   - Type 'R' to preview rotation patterns
   - Select pieces by number (1-13) or random (0)
3. **Get recommendations**: Bot displays optimal moves with:
   - Visual move cards showing placement and rotation
   - Final board preview after all moves
   - Score impact for each piece

### Visual Piece Selection

The bot features an interactive piece gallery:

```
======================================================================
  PIECE GALLERY - Select from available shapes
======================================================================
  [1] Single        [2] Dot 2         [3] Dot 3
    ██                ████              ██████

  [4] Dot 4         [5] Dot 5         [6] Square 2x2
    ████████          ██████████        ████
                                        ████
...
  [0] Random piece
  [R] Show rotations for a piece
======================================================================
```

**Rotation Preview**: Press 'R' to see all rotation patterns before selecting!

### Automatic Mode

```bash
# Run 10 automatic games
./block-blast-bot --auto 10
```

### Command-Line Options

```
-h, --help              Show help message
-v, --version           Show version
-a, --auto <N>          Run N automatic games
-q, --quiet             Quiet mode
--beam-width <N>        Set beam search width (default: 50)
--max-depth <N>         Set search depth (default: 3)
--threads <N>           Set number of threads (default: auto)
--no-parallel           Disable parallel processing
```

## Algorithm

The bot uses **Beam Search** with the following strategy:

1. **Move Generation**: Generate all valid placements for each piece
2. **Evaluation**: Score each state using weighted criteria:
   - Empty space (more = better)
   - Combo potential (line clearing)
   - Survival rate (future move availability)
   - Penalties for holes and height variance
3. **Beam Search**: Keep top N states at each depth level
4. **Pruning**: Remove states below score threshold

### Scoring Formula

```
Score = α×empty_space + β×combo + γ×survival - δ×holes - ε×height_variance
```

Default weights:

- α (empty space) = 1.0
- β (combo) = 10.0
- γ (survival) = 5.0
- δ (holes) = -2.0
- ε (height variance) = -0.5

## Performance

### Optimizations

- **Bitset representation**: Fast board operations
- **Beam pruning**: Reduces search space
- **Move ordering**: Better moves evaluated first
- **Rotation caching**: Pre-compute piece rotations

### Benchmarks (estimated)

- Simple positions: < 100ms
- Complex positions: 100-500ms
- Beam width 50, depth 3: ~10K-50K nodes evaluated

## Customization

### Adding New Piece Shapes

Edit `src/core/Piece.cpp`:

```cpp
case Shape::NEW_SHAPE:
    return {{0, 0}, {1, 0}, {0, 1}};  // Define cells
```

### Tuning Scoring Weights

Modify in `include/common.h`:

```cpp
struct ScoringWeights {
    float emptySpaceWeight = 1.0f;
    float comboWeight = 10.0f;
    // ... adjust as needed
};
```

### Adjusting Search Parameters

```cpp
Solver::Config config;
config.beamWidth = 100;      // More thorough search
config.maxDepth = 4;         // Deeper lookahead
config.pruningThreshold = 0.5f;  // More aggressive pruning
config.useParallel = true;   // Enable multicore (default)
config.numThreads = 4;       // Or 0 for auto-detect
```

## Multithreading / Multicore Support

The bot supports **OpenMP parallelization** for 2-5x performance improvement on multi-core systems.

### Quick Start

```bash
# Default (uses all available cores if OpenMP enabled)
./block-blast-bot

# Specify thread count
./block-blast-bot --threads 4

# Disable parallelization
./block-blast-bot --no-parallel
```

### Performance Impact

- **2 cores**: 1.5-1.8x speedup
- **4 cores**: 2.5-3.5x speedup
- **8 cores**: 3.5-5.0x speedup
- **16+ cores**: 4.0-6.0x speedup

See [MULTITHREADING.md](MULTITHREADING.md) for complete details.
See [MACOS_OPENMP.md](MACOS_OPENMP.md) for macOS setup.

## Development

### Code Style

- Modern C++17 features
- RAII and smart pointers
- Const-correctness
- Comprehensive comments

### Testing

```bash
cd build
make test_bot
./test_bot

# Demo programs
./piece_selection_demo  # View piece gallery and rotations
./visual_demo          # See visual display improvements
```

## Quick Reference

### Piece Selection Commands

- `1-13`: Select specific piece shape
- `0`: Random piece
- `R`: View rotation patterns
- `Enter`: Confirm selection

### Available Pieces

1. Single (1x1) - Single block
2. Dot 2 (2x1) - Two horizontal blocks
3. Dot 3 (3x1) - Three horizontal blocks
4. Dot 4 (4x1) - Four horizontal blocks
5. Dot 5 (5x1) - Five horizontal blocks
6. Square 2x2 - 2x2 square
7. Square 3x3 - 3x3 square
8. L Small - Small L shape
9. L Large - Large L shape
10. T Shape - T-shaped piece
11. Z Shape - Z-shaped piece
12. Plus - Plus/cross shape
13. Corner 3x3 - Corner piece

## Documentation

- **[PIECE_SELECTION.md](PIECE_SELECTION.md)** - Visual piece selection interface guide
- **[VISUAL_IMPROVEMENTS.md](VISUAL_IMPROVEMENTS.md)** - Display enhancements documentation
- **[MULTITHREADING.md](MULTITHREADING.md)** - Parallel processing details
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System architecture overview

## Future Enhancements

- [ ] Neural network evaluation function
- [ ] MCTS (Monte Carlo Tree Search) solver
- [x] ~~Parallel beam search with OpenMP~~ **✅ Implemented!**
- [ ] GUI visualization
- [ ] Database of opening patterns
- [ ] Machine learning for weight optimization

## License

MIT License - see LICENSE file for details

## Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Add tests for new features
4. Submit a pull request

## Author

Created for high-performance Block Blast puzzle solving.
