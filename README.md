# game

Simple 2D game written in C using SDL2, no engine or framework.

## Dependencies

- GCC or Clang
- CMake 3.16+
- SDL2

### Installing SDL2

**Linux (Debian/Ubuntu)**
```bash
sudo apt install libsdl2-dev
```

**macOS**
```bash
brew install sdl2
```

**Windows**
Download the development libraries from https://libsdl.org and point CMake to them.

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

```bash
./build/game
```

## Project structure

```
.
├── CMakeLists.txt
├── src/
│   └── main.c
└── assets/
```