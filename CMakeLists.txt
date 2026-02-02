cmake_minimum_required(VERSION 3.15)
project(BlockBlastBot VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for optimization
if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        add_compile_options(/O2 /W4)
    else()
        add_compile_options(-O3 -march=native -Wall -Wextra -Wpedantic)
    endif()
else()
    if(MSVC)
        add_compile_options(/W4 /DEBUG)
    else()
        add_compile_options(-g -Wall -Wextra -Wpedantic)
    endif()
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Source files
set(CORE_SOURCES
    src/core/Piece.cpp
    src/core/Board.cpp
    src/core/GameState.cpp
)

set(GAME_SOURCES
    src/game/Evaluator.cpp
)

set(AI_SOURCES
    src/ai/Solver.cpp
)

set(MAIN_SOURCES
    src/BotController.cpp
    src/main.cpp
)

# Create executable
add_executable(block-blast-bot
    ${CORE_SOURCES}
    ${GAME_SOURCES}
    ${AI_SOURCES}
    ${MAIN_SOURCES}
)

# Optional: Enable OpenMP for parallel processing
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(block-blast-bot PUBLIC OpenMP::OpenMP_CXX)
    add_compile_definitions(USE_OPENMP)
endif()

# Example executable
add_executable(example
    src/example.cpp
    ${CORE_SOURCES}
    ${GAME_SOURCES}
    ${AI_SOURCES}
    src/BotController.cpp
)

# Installation
install(TARGETS block-blast-bot DESTINATION bin)

# Testing (optional)
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    
    # Add test executable
    add_executable(test_bot
        tests/test_board.cpp
        ${CORE_SOURCES}
        ${GAME_SOURCES}
        ${AI_SOURCES}
    )
    
    add_test(NAME BoardTests COMMAND test_bot)
endif()

# Print configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
