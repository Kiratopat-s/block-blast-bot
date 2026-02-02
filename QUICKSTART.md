# Quick Start Guide

## Build & Run

```bash
# Build the project
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# Run tests
./test_bot

# Run interactive bot
./block-blast-bot

# Run automatic simulation
./block-blast-bot --auto 10
```

## Example Usage

### Interactive Mode

```
=== Block Blast Bot ===
Options:
1. Enter new board state
2. Use empty board
3. Quit
Choice: 2

Enter 3 pieces:
Piece 1: 6    # Square 2x2
Piece 2: 3    # Dot 3
Piece 3: 8    # L Small

Computing best moves...
Best Move Sequence:
  Move 1: Piece 1 at (0, 0)
  Move 2: Piece 2 at (2, 0)
  Move 3: Piece 3 at (0, 2)
```

## Piece Types Reference

| ID  | Shape   | Description         |
| --- | ------- | ------------------- |
| 1   | Single  | 1x1 block           |
| 2   | Dot2    | 2 horizontal blocks |
| 3   | Dot3    | 3 horizontal blocks |
| 4   | Dot4    | 4 horizontal blocks |
| 5   | Dot5    | 5 horizontal blocks |
| 6   | Square2 | 2x2 square          |
| 7   | Square3 | 3x3 square          |
| 8   | L_Small | Small L shape       |
| 9   | L_Large | Large L shape       |
| 10  | T       | T shape             |
| 11  | Z       | Z shape             |
| 12  | Plus    | + shape             |
| 13  | Corner3 | 3x3 corner          |

## Advanced Configuration

### Tuning Performance

```cpp
Solver::Config config;
config.beamWidth = 100;      // Increase for better quality (slower)
config.maxDepth = 4;         // Deeper search (slower)
config.pruningThreshold = 0.5f;  // More aggressive pruning

BotController bot(config);
```

### Adjusting Scoring Weights

```cpp
ScoringWeights weights;
weights.emptySpaceWeight = 2.0f;  // Prefer more empty space
weights.comboWeight = 15.0f;      // Prioritize combos
weights.survivalWeight = 8.0f;    // Focus on future moves

config.weights = weights;
```

## Performance Tips

1. **Release Build**: Always use `-DCMAKE_BUILD_TYPE=Release` for 10-100x speedup
2. **Beam Width**: Start with 50, increase to 100+ for critical decisions
3. **Max Depth**: 3 is usually sufficient, 4+ for endgame analysis
4. **Pruning**: Lower threshold (0.2-0.3) for faster search

## Troubleshooting

### Build Errors

- Ensure C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Check CMake version ≥ 3.15
- On macOS: `xcode-select --install`

### Slow Performance

- Verify Release build: `cmake -DCMAKE_BUILD_TYPE=Release`
- Reduce beam width or max depth
- Enable compiler optimizations: `-O3 -march=native`

### Memory Issues

- Reduce beam width to 20-30
- Decrease max depth to 2
- Limit max sequences in move generator
