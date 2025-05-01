# RBud - Virtual Pet Application

RBud is a virtual pet application built using the Raylib library and Windows API. It features a draggable pet that interacts with the desktop environment.

## Features

- **Virtual Pet**: A draggable pet that responds to user interactions.
- **Desktop Integration**: The pet interacts with desktop windows and respects screen boundaries.
- **Customizable Assets**: Easily replace the pet sprite with your own.

## Requirements

- **Operating System**: Windows
- **Dependencies**:
  - [Raylib](https://www.raylib.com/)
  - Windows API libraries (`gdi32`, `winmm`, `dwmapi`)
  - `pthread` library

## Build Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd RBud
   ```

2. Install Raylib and other dependencies.

3. Build the project using the provided `Makefile`:
   ```bash
   make
   ```

4. Run the application:
   ```bash
   ./build/game
   ```

## File Structure

- **src/**: Contains the source code files.
- **include/**: Contains header files.
- **res/**: Contains resources like the pet sprite (`kai.png`).
- **build/**: Directory for compiled binaries and resources (created during the build process).