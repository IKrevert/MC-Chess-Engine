# MC-Chess-Engine

## Overview
MC-Chess-Engine is a C++ chess engine designed to interface directly with hardware-based inputs. In its primary use case, the engine runs alongside a microcontroller connected to an **8×8 matrix of Hall effect sensors**, allowing for a physical chessboard to serve as the user interface.

The microcontroller’s job is to scan the sensor matrix, detect changes in piece positions, and send those moves to the engine for processing. The engine then validates, applies, and tracks the moves according to the rules of chess.

Although tailored for a physical chessboard, the engine is flexible and can be adapted to graphical or terminal-based interfaces for testing or alternative applications.

---

## How It Works

1. **Sensor Input (Microcontroller)**  
   Each square on the board has a Hall effect sensor that detects magnetic fields from embedded magnets in the chess pieces.  
   - The microcontroller reads all 64 sensors in real-time.  
   - State changes (piece lifted or placed) are detected and recorded.

2. **Move Detection**  
   From the change in sensor readings, the microcontroller determines the origin and destination squares of the move.

3. **Move Validation**  
   The engine receives the move, generates all legal moves for the current position, and checks if the input matches a legal move.

4. **Game State Update**  
   Valid moves are applied to the internal board representation, including updates to castling rights, en passant possibilities, and position hash keys.

5. **Output**  
   The validated move can be:  
   - Displayed on a connected device  
   - Sent to a GUI over serial or network connection  
   - Logged for analysis

---

## Features
- Full legal move generation with bitboards
- Detection of check, checkmate, stalemate, and draws
- Support for castling and en passant
- Position hashing via Zobrist keys
- Hardware-friendly design with minimal dependencies
- Easily portable to other embedded systems or desktop environments

---

## Potential Uses
- Physical chessboards with digital tracking
- Training boards for chess education
- Remote-play enabled boards
- Embedded chess AI projects

---

## Documentation
Full API and code documentation is generated with **Doxygen**.  
You can browse the HTML version here:  
**Live Docs:** [View on GitHub Pages](https://your-username.github.io/your-repo/)  

If you cloned the repo, documentation can be found in:

docs/html/index.html 
