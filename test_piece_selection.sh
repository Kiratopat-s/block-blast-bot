#!/bin/bash
# Test the new visual piece selection interface

cd "$(dirname "$0")/build"

echo "Testing Visual Piece Selection Interface"
echo "========================================"
echo ""
echo "This will demonstrate:"
echo "1. Visual piece gallery"
echo "2. Rotation pattern viewer"
echo "3. Interactive piece selection"
echo ""
echo "Commands to try:"
echo "  - Type 'R' to view rotation patterns"
echo "  - Type 1-13 to select a specific piece"
echo "  - Type 0 for a random piece"
echo ""
echo "Press Enter to start..."
read

# Simulate selecting pieces with the visual interface
# In real usage, user interacts directly
./block-blast-bot
