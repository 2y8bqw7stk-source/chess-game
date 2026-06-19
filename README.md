# ♟️ Chess Game — C++ / Qt6

A fully featured chess game built with **C++** and **Qt6**, featuring a clean graphical interface and a built-in AI opponent.

## Features

- **Two game modes** — Player vs Player and Player vs AI
- **AI opponent** powered by the Minimax algorithm with Alpha-Beta pruning (depth 2–3)
- **Difficulty levels** — Easy, Medium, Hard
- **Full rule support**:
  - Castling (kingside & queenside)
  - En passant
  - Pawn promotion (choose your piece)
  - Check & checkmate detection
  - Stalemate
  - 50-move rule
  - Threefold repetition
- **Visual highlights** — selected piece, legal moves, captures, check
- **Chess timer** — optional per-player countdown
- **Board flip** — automatically flips when playing as Black
- **Fullscreen mode** — toggle with F11

## Requirements

- Qt 6.x (or Qt 5.x with minor adjustments)
- C++17 compatible compiler (GCC, Clang, MSVC)
- qmake or CMake

## Build & Run

```bash
# Clone the repository
git clone https://github.com/your-username/chess-game.git
cd chess-game

# Build with qmake
qmake chess.pro
make

# Run
./chess-game
```

## Project Structure

```
├── main.cpp          # Entry point
├── MainWindow.h/cpp  # Main window, UI, timer, dialogs
├── ChessBoard.h/cpp  # Board rendering, input handling, AI logic
├── Plateau.h/cpp     # Game logic (moves, rules, board state)
└── chess.pro         # Qt project file
```

## How It Works

- `Plateau` handles the pure game logic — move validation, board state, draw conditions, and position evaluation.
- `ChessBoard` is the Qt widget responsible for rendering and user interaction, and runs the AI via Minimax.
- `MainWindow` wraps everything into a polished UI with menus, a status bar, and a chess clock.

- ## 👥 Team

     | Name | Role |
     |------|------|
     | Ali Alami Marktani | Developer |
     | Noha Mahfoudi | Developer |
     | Abir Badaoui | Developer |

- ## 📄 License

  This project was developed as part of the C++ module at ESISA — 2025/2026.


