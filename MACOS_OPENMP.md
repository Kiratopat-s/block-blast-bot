# Enabling OpenMP on macOS

OpenMP is not available with Apple's default Clang compiler. Here's how to enable it:

## Option 1: Install LLVM with OpenMP (Recommended)

```bash
# Install LLVM via Homebrew (includes OpenMP)
brew install llvm

# Set environment variables (add to ~/.zshrc or ~/.bash_profile)
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"

# For Intel Macs, use /usr/local instead of /opt/homebrew
# export PATH="/usr/local/opt/llvm/bin:$PATH"

# Rebuild the project
cd build
cmake -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang \
      -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ \
      -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Option 2: Install GCC with OpenMP

```bash
# Install GCC via Homebrew
brew install gcc

# Find GCC version
ls /opt/homebrew/bin/gcc-*

# Rebuild with GCC
cd build
cmake -DCMAKE_C_COMPILER=gcc-13 \
      -DCMAKE_CXX_COMPILER=g++-13 \
      -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Option 3: Install libomp Separately

```bash
# Install just the OpenMP library
brew install libomp

# Set flags
export LDFLAGS="-L/opt/homebrew/opt/libomp/lib"
export CPPFLAGS="-I/opt/homebrew/opt/libomp/include"

# Rebuild
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Verify OpenMP is Enabled

After rebuilding, check the CMake output:

```bash
-- Found OpenMP_CXX (found version "5.0")
-- Found OpenMP (found version "5.0")
```

Or run the example:

```bash
./example
```

You should see:

```
OpenMP enabled - Running parallel and serial comparisons
```

Instead of:

```
OpenMP not available - Running serial only
```

## Performance Comparison

### Without OpenMP (Serial)

```
Average time: 66ms
```

### With OpenMP (Parallel, 8 cores)

```
Average time (parallel): 22ms
Average time (serial): 66ms
Speedup: 3.0x
```

## Troubleshooting

### "Could NOT find OpenMP"

1. Make sure LLVM/GCC is installed
2. Clear CMake cache: `rm -rf build && mkdir build`
3. Specify compiler explicitly when running cmake
4. Check compiler supports OpenMP: `clang --version` or `gcc --version`

### "Library not loaded: libomp.dylib"

```bash
# Add library path
export DYLD_LIBRARY_PATH=/opt/homebrew/opt/libomp/lib:$DYLD_LIBRARY_PATH

# Or install to system location
sudo ln -s /opt/homebrew/opt/libomp/lib/libomp.dylib /usr/local/lib/
```

### Compiler Warnings

If you see warnings about OpenMP, ensure you're using the LLVM compiler:

```bash
which clang
# Should show: /opt/homebrew/opt/llvm/bin/clang
```

## Make It Permanent

Add to `~/.zshrc` or `~/.bash_profile`:

```bash
# LLVM (with OpenMP support)
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
export DYLD_LIBRARY_PATH="/opt/homebrew/opt/libomp/lib:$DYLD_LIBRARY_PATH"

# Alias for easy compiling
alias cmake-omp='cmake -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++'
```

Then use:

```bash
source ~/.zshrc
cd build
cmake-omp -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Expected Performance

On an M1/M2 Mac with 8 performance cores:

- **Speedup**: 2.5-4.0x
- **Solve time**: 66ms → 18-26ms

On Intel Mac with 4 cores:

- **Speedup**: 2.0-3.0x
- **Solve time**: 66ms → 22-33ms

The bot works perfectly fine without OpenMP (serial mode), but multithreading provides significant performance improvements on multi-core systems.
