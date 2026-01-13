# OrderBook

A high-performance C++ order book implementation for trading systems.

## Building

### Prerequisites
- CMake 3.20 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build . -j$(nproc)

# Run tests (if enabled)
ctest
```

### Build Options

- `BUILD_TESTS`: Build test suite (default: ON)
- `BUILD_EXAMPLES`: Build example programs (default: ON)

Example:
```bash
cmake -DBUILD_TESTS=OFF ..
```

## Project Structure

```
OrderBook/
├── CMakeLists.txt      # Main CMake configuration
├── include/            # Header files
├── src/                # Source files
├── tests/              # Test files
├── examples/           # Example programs
└── README.md           # This file
```

## Usage

Include headers from the `include/` directory:

```cpp
#include <orderbook/OrderBook.hpp>
```

## Development

This project follows modern C++ best practices:
- C++17 standard
- Strict compiler warnings
- Optimized builds (-O3, -march=native)
- Static analysis ready
