#include "Solver.h"
#include <algorithm>
#include <chrono>
#include <limits>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace BlockBlast {

Solver::Solver(const Config& config) 
    : config_(config), evaluator_(config.weights) {
    #ifdef _OPENMP
    // Configure OpenMP thread count
    if (config_.numThreads > 0) {
        omp_set_num_threads(config_.numThreads);
    }
    // else use default (usually number of CPU cores)
    #endif
}

Solver::Solver() 
    : Solver(Config()) {}

MoveSequence Solver::findBestSequence(GameState& state) {
    auto startTime = std::chrono::high_resolution_clock::now();
    resetStats();
    
    auto bestNodes = beamSearch(state);
    
    auto endTime = std::chrono::high_resolution_clock::now();
    stats_.timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime).count();
    
    if (bestNodes.empty()) {
        return MoveSequence();
    }
    
    stats_.bestScore = bestNodes[0].score;
    return bestNodes[0].sequence;
}

Move Solver::findBestMove(const GameState& state, int pieceIndex) {
    if (pieceIndex < 0 || pieceIndex >= PIECES_PER_TURN) {
        return Move();
    }
    
    if (state.isPieceUsed(pieceIndex)) {
        return Move();
    }
    
    const Piece& piece = state.getPiece(pieceIndex);
    auto moves = moveGenerator_.generateMoves(state.getBoard(), piece, pieceIndex);
    
    if (moves.empty()) {
        return Move();
    }
    
    Move bestMove;
    float bestScore = -std::numeric_limits<float>::infinity();
    
    // Parallel evaluation of moves if OpenMP is available
    #ifdef _OPENMP
    if (config_.useParallel && moves.size() > 10) {
        #pragma omp parallel
        {
            Move localBestMove;
            float localBestScore = -std::numeric_limits<float>::infinity();
            
            #pragma omp for nowait
            for (size_t i = 0; i < moves.size(); ++i) {
                GameState tempState = state.copy();
                if (tempState.executeMove(moves[i])) {
                    float score = evaluator_.evaluate(tempState);
                    if (score > localBestScore) {
                        localBestScore = score;
                        localBestMove = moves[i];
                        localBestMove.score = score;
                    }
                }
            }
            
            #pragma omp critical
            {
                if (localBestScore > bestScore) {
                    bestScore = localBestScore;
                    bestMove = localBestMove;
                }
            }
        }
    } else
    #endif
    {
        // Serial version
        GameState tempState = state.copy();
        for (auto& move : moves) {
            tempState = state.copy();
            if (tempState.executeMove(move)) {
                float score = evaluator_.evaluate(tempState);
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                    bestMove.score = score;
                }
            }
        }
    }
    
    return bestMove;
}

std::vector<Solver::SearchNode> Solver::beamSearch(GameState& initialState) {
    std::vector<SearchNode> currentBeam;
    
    // Initialize with root node
    SearchNode root;
    root.state = initialState.copy();
    root.depth = 0;
    evaluateNode(root);
    currentBeam.push_back(root);
    
    // Search until max depth or all pieces placed
    for (int depth = 0; depth < config_.maxDepth; ++depth) {
        std::vector<SearchNode> nextBeam;
        
        // Expand each node in current beam
        #ifdef _OPENMP
        if (config_.useParallel && currentBeam.size() > 5) {
            // Parallel expansion
            std::vector<std::vector<SearchNode>> threadResults(currentBeam.size());
            
            #pragma omp parallel for schedule(dynamic)
            for (size_t i = 0; i < currentBeam.size(); ++i) {
                const auto& node = currentBeam[i];
                
                if (node.state.getRemainingPieces() == 0) {
                    threadResults[i].push_back(node);
                } else {
                    threadResults[i] = expandNode(node);
                }
            }
            
            // Merge results
            for (auto& result : threadResults) {
                stats_.nodesGenerated += result.size();
                nextBeam.insert(nextBeam.end(), result.begin(), result.end());
            }
        } else
        #endif
        {
            // Serial expansion
            for (const auto& node : currentBeam) {
                // Skip if all pieces are used
                if (node.state.getRemainingPieces() == 0) {
                    nextBeam.push_back(node);
                    continue;
                }
                
                auto children = expandNode(node);
                stats_.nodesGenerated += children.size();
                
                nextBeam.insert(nextBeam.end(), children.begin(), children.end());
            }
        }
        
        // Prune and keep best nodes
        pruneNodes(nextBeam);
        
        // Keep top beamWidth nodes
        if (nextBeam.size() > static_cast<size_t>(config_.beamWidth)) {
            std::partial_sort(nextBeam.begin(), 
                            nextBeam.begin() + config_.beamWidth,
                            nextBeam.end());
            nextBeam.resize(config_.beamWidth);
        } else {
            std::sort(nextBeam.begin(), nextBeam.end());
        }
        
        currentBeam = std::move(nextBeam);
        
        if (currentBeam.empty()) break;
    }
    
    return currentBeam;
}

void Solver::evaluateNode(SearchNode& node) {
    node.score = evaluator_.evaluate(node.state);
    stats_.nodesEvaluated++;
}

std::vector<Solver::SearchNode> Solver::expandNode(const SearchNode& node) {
    std::vector<SearchNode> children;
    
    // Try each unused piece
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (node.state.isPieceUsed(i)) continue;
        
        const Piece& piece = node.state.getPiece(i);
        auto moves = moveGenerator_.generateMoves(node.state.getBoard(), piece, i);
        
        for (const auto& move : moves) {
            SearchNode child;
            child.state = node.state.copy();
            child.sequence = node.sequence;
            child.depth = node.depth + 1;
            
            if (child.state.executeMove(move)) {
                child.sequence.moves[child.sequence.piecesPlaced - 1] = move;
                child.sequence.piecesPlaced++;
                
                evaluateNode(child);
                children.push_back(child);
            }
        }
    }
    
    return children;
}

void Solver::pruneNodes(std::vector<SearchNode>& nodes) {
    if (nodes.empty()) return;
    
    // Find best score
    float maxScore = -std::numeric_limits<float>::infinity();
    for (const auto& node : nodes) {
        maxScore = std::max(maxScore, node.score);
    }
    
    // Remove nodes below threshold
    float threshold = maxScore * config_.pruningThreshold;
    nodes.erase(
        std::remove_if(nodes.begin(), nodes.end(),
            [threshold](const SearchNode& node) {
                return node.score < threshold;
            }),
        nodes.end()
    );
}

MoveSequence Solver::solveIterativeDeepening(GameState& state, int maxTime) {
    auto startTime = std::chrono::high_resolution_clock::now();
    
    MoveSequence bestSequence;
    float bestScore = -std::numeric_limits<float>::infinity();
    
    // Try increasing depths
    for (int depth = 1; depth <= config_.maxDepth; ++depth) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime).count();
        
        if (elapsed >= maxTime) break;
        
        // Set current depth
        int originalDepth = config_.maxDepth;
        config_.maxDepth = depth;
        
        auto sequence = findBestSequence(state);
        
        if (sequence.totalScore > bestScore) {
            bestScore = sequence.totalScore;
            bestSequence = sequence;
        }
        
        // Restore original depth
        config_.maxDepth = originalDepth;
    }
    
    return bestSequence;
}

// GreedySolver implementations
GreedySolver::GreedySolver(const ScoringWeights& weights) 
    : evaluator_(weights) {}

MoveSequence GreedySolver::solve(GameState& state) {
    MoveSequence sequence;
    
    // Greedily place each piece
    for (int i = 0; i < PIECES_PER_TURN; ++i) {
        if (state.isPieceUsed(i)) continue;
        
        const Piece& piece = state.getPiece(i);
        auto moves = moveGenerator_.generateMoves(state.getBoard(), piece, i);
        
        if (moves.empty()) continue;
        
        // Find best move for this piece
        Move bestMove;
        float bestScore = -std::numeric_limits<float>::infinity();
        
        for (const auto& move : moves) {
            GameState tempState = state.copy();
            if (tempState.executeMove(move)) {
                float score = evaluator_.evaluate(tempState);
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
        }
        
        // Execute best move
        if (bestMove.pieceIndex >= 0) {
            state.executeMove(bestMove);
            sequence.moves[sequence.piecesPlaced] = bestMove;
            sequence.piecesPlaced++;
            sequence.totalScore += bestScore;
        }
    }
    
    return sequence;
}

} // namespace BlockBlast
