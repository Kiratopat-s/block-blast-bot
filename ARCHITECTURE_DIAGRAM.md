# Block Blast Bot - Visual Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                      USER INTERFACE                              │
│  ┌────────────────┐  ┌──────────────┐  ┌──────────────────┐    │
│  │  Interactive   │  │  Automatic   │  │   Example &      │    │
│  │     Mode       │  │    Mode      │  │   Benchmark      │    │
│  │   (main.cpp)   │  │              │  │  (example.cpp)   │    │
│  └────────┬───────┘  └──────┬───────┘  └────────┬─────────┘    │
│           └──────────────────┴──────────────────┬┘              │
└────────────────────────────────────────────────┼────────────────┘
                                                  │
                                                  ▼
┌─────────────────────────────────────────────────────────────────┐
│                    BOT CONTROLLER                                │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  BotController (BotController.h/cpp)                     │   │
│  │  • Game loop management                                  │   │
│  │  • Input/Output handling                                 │   │
│  │  • CLI argument parsing                                  │   │
│  │  • Statistics display                                    │   │
│  └────┬─────────────────────────────────────────────────────┘   │
└───────┼──────────────────────────────────────────────────────────┘
        │
        ▼
┌─────────────────────────────────────────────────────────────────┐
│                      AI LAYER                                    │
│  ┌────────────────────────────┐  ┌───────────────────────────┐  │
│  │  Solver (Solver.h/cpp)     │  │  GreedySolver             │  │
│  │  ┌──────────────────────┐  │  │  • Fast baseline          │  │
│  │  │  Beam Search         │  │  │  • Sequential placement   │  │
│  │  │  • Width: 50         │  │  │  • < 1ms solve time       │  │
│  │  │  • Depth: 3          │  │  └───────────────────────────┘  │
│  │  │  • Pruning: 0.3      │  │                                 │
│  │  │  • ~66ms avg         │  │                                 │
│  │  └──────────────────────┘  │                                 │
│  └──────────┬─────────────────┘                                 │
└─────────────┼───────────────────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────────────────────────────────┐
│                   GAME LOGIC LAYER                               │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  Evaluator (Evaluator.h/cpp)                             │   │
│  │  ┌────────────────┐  ┌────────────────┐  ┌────────────┐ │   │
│  │  │ Empty Space    │  │    Combo       │  │  Survival  │ │   │
│  │  │  Score (α)     │  │  Score (β)     │  │  Rate (γ)  │ │   │
│  │  │  Weight: 1.0   │  │  Weight: 10.0  │  │ Weight: 5.0│ │   │
│  │  └────────────────┘  └────────────────┘  └────────────┘ │   │
│  │  ┌────────────────┐  ┌────────────────┐                 │   │
│  │  │  Hole Penalty  │  │ Height Variance│                 │   │
│  │  │  Weight: -2.0  │  │  Weight: -0.5  │                 │   │
│  │  └────────────────┘  └────────────────┘                 │   │
│  └──────────────────────────────────────────────────────────┘   │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  MoveGenerator (Evaluator.h/cpp)                         │   │
│  │  • Generate all valid placements                         │   │
│  │  • Consider all rotations                                │   │
│  │  • Sequence generation (order matters)                   │   │
│  │  • Pruning strategies                                    │   │
│  └──────────┬───────────────────────────────────────────────┘   │
└─────────────┼───────────────────────────────────────────────────┘
              │
              ▼
┌─────────────────────────────────────────────────────────────────┐
│                     CORE LAYER                                   │
│  ┌─────────────────────┐  ┌────────────────────────────────┐   │
│  │  Board              │  │  GameState                     │   │
│  │  (Board.h/cpp)      │  │  (GameState.h/cpp)             │   │
│  │  ┌───────────────┐  │  │  ┌──────────────────────────┐ │   │
│  │  │ 8x8 Bitset    │  │  │  │  • Board state           │ │   │
│  │  │ (uint64_t)    │  │  │  │  • 3 pieces              │ │   │
│  │  │               │  │  │  │  • Score tracking        │ │   │
│  │  │ ███████░░     │  │  │  │  • Combo counter         │ │   │
│  │  │ ░░░░░░░░░     │  │  │  │  • Move history          │ │   │
│  │  │ ░██████░░     │  │  │  └──────────────────────────┘ │   │
│  │  │ ░░░░░░░░░     │  │  └────────────────────────────────┘   │
│  │  └───────────────┘  │                                        │
│  │  • O(1) cell ops    │                                        │
│  │  • Fast line clear  │                                        │
│  │  └─────────────────┘                                         │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  Piece (Piece.h/cpp)                                     │   │
│  │  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  ┌─────┐  │   │
│  │  │ ■   │  │ ■ ■ │  │ ■ ■ │  │ ■   │  │ ■ ■ │  │  ■  │  │   │
│  │  │     │  │     │  │ ■ ■ │  │ ■   │  │ ■ ■ │  │ ■■■ │  │   │
│  │  │1x1  │  │1x2  │  │2x2  │  │ ■   │  │     │  │  ■  │  │   │
│  │  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  └─────┘  │   │
│  │  13 predefined shapes + rotations (0-3)                 │   │
│  │  • PieceFactory for creation                            │   │
│  │  • Automatic normalization                              │   │
│  └──────────────────────────────────────────────────────────┘   │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  Common (common.h)                                       │   │
│  │  • Position, Move, MoveSequence types                    │   │
│  │  • ScoringWeights configuration                         │   │
│  │  • Constants (BOARD_SIZE=8, PIECES_PER_TURN=3)          │   │
│  └──────────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────┘
```

## Data Flow

```
User Input → BotController → Solver → Evaluator → GameState → Board
                    ↓            ↓         ↓          ↓         ↓
                CLI Parse   Beam Search  Score   Track State  Bitset
                    ↓            ↓         ↓          ↓         ↓
            Create Pieces  Gen Moves  Evaluate   Execute    Place/Clear
                    ↓            ↓         ↓          ↓         ↓
             GameState.setPieces() → solver.findBest() → Result
```

## Performance Optimization Points

```
┌──────────────────────┐
│  Bitset Operations   │ ← O(1) cell access, line checks
└──────────────────────┘
           ↓
┌──────────────────────┐
│  Beam Pruning        │ ← Reduce search space by 50-70%
└──────────────────────┘
           ↓
┌──────────────────────┐
│  Rotation Caching    │ ← Pre-compute all rotations
└──────────────────────┘
           ↓
┌──────────────────────┐
│  Move Ordering       │ ← Best moves first (alpha-beta)
└──────────────────────┘
           ↓
┌──────────────────────┐
│  Compiler -O3        │ ← Aggressive optimizations
└──────────────────────┘
```

## Algorithm Complexity

| Component        | Time Complexity | Space Complexity |
| ---------------- | --------------- | ---------------- |
| Board operations | O(1)            | O(1)             |
| Piece placement  | O(k)            | O(1)             |
| Line clearing    | O(n)            | O(1)             |
| Move generation  | O(n²×r)         | O(n²×r)          |
| Beam search      | O(b×d×m)        | O(b)             |
| Full solve       | O(b×d×n²×r)     | O(b×d)           |

Where:

- k = piece size (≤25)
- n = board size (8)
- r = rotations (≤4)
- b = beam width (50)
- d = depth (3)
- m = moves per piece (~64)

## Memory Layout

```
GameState (64 bytes)
├── Board (8 bytes)          ← uint64_t bitset
├── Pieces[3] (192 bytes)    ← 3 × piece data
├── PiecesUsed[3] (3 bytes)  ← bool flags
├── Score (4 bytes)          ← int32_t
└── Combo (4 bytes)          ← int32_t

Total per state: ~280 bytes
Beam width 50: ~14 KB memory
```

## Execution Flow

```
1. main() → Parse CLI
              ↓
2. Create BotController(config)
              ↓
3. Interactive/Auto mode
              ↓
4. setPieces(3 pieces)
              ↓
5. solver.findBestSequence()
              ↓
6. ┌─── Beam Search Loop ───┐
   │ For each depth (0-3):  │
   │   Generate moves       │
   │   Evaluate positions   │
   │   Prune bad states     │
   │   Keep top 50          │
   └────────────────────────┘
              ↓
7. Return best MoveSequence
              ↓
8. executeSequence()
              ↓
9. Display results & stats
```

This architecture ensures:

- **Performance**: Bitboards, pruning, optimizations
- **Modularity**: Clear layer separation
- **Extensibility**: Easy to add features
- **Testability**: Unit testable components
- **Maintainability**: Clean code patterns
