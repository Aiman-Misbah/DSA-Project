<div align="center">

# 🎮 DSA-Project

### A Tetris-style game built in C++ with custom data structures

<p>
  <strong>Data Structures &nbsp;•&nbsp; Game Development &nbsp;•&nbsp; Raylib</strong>
</p>

<p>
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17">
  <img src="https://img.shields.io/badge/Raylib-5.5-000000?style=for-the-badge&logo=raylib&logoColor=white" alt="Raylib 5.5">
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Windows">
</p>

</div>

---

## 📌 Overview

**DSA-Project** is a Tetris-style desktop game developed in **C++** using **Raylib**.

The project was designed to demonstrate how fundamental **data structures can be implemented from scratch and integrated into a complete interactive application**.

Instead of treating data structures as isolated exercises, each structure has a specific role within the game:

* A **linked list** manages the piece bag and game-board rows.
* A **queue** manages upcoming pieces.
* A **stack** stores previous game states for undo functionality.
* An **AVL tree** organises leaderboard scores.
* A dedicated **PieceQueue** combines multiple structures to manage piece generation.

This makes the project both a playable game and a practical demonstration of data-structure usage.

---

## ✨ Features

* 🎮 Tetris-style gameplay
* 🧩 Seven standard Tetris piece types
* 🔄 Piece movement and rotation
* 💥 Collision detection
* 🧱 Completed-row detection and clearing
* 👀 Upcoming-piece preview
* 🎲 Random piece generation using a piece bag
* ↩️ Undo functionality through saved game states
* 🏆 AVL-tree-based score leaderboard
* 🔊 Background music and sound effects
* 🖥️ Raylib-based graphical interface
* 📦 Modular source and header organisation

---

## 🧠 Data Structures

The core of the project is its custom data-structure implementation.

### 1. Linked List — Piece Bag

The piece bag is implemented using a custom **singly linked list**.

Each node stores a `Piece` object and a pointer to the next node.

```text
LinkedList
    │
    ▼
┌─────────────┐
│ Piece       │
│ next ───────┼──────► Piece
└─────────────┘          │
                         ▼
                       Piece
                         │
                         ▼
                        ...
```

The `LinkedList` implementation provides operations for:

* Adding pieces
* Retrieving pieces by index
* Removing pieces
* Checking the current size
* Clearing the list

The list is used by `PieceQueue` as the **piece bag** from which new pieces are selected.

---

### 2. Queue — Upcoming Pieces

A custom **FIFO queue** manages pieces waiting to enter the game.

The queue has a default capacity of **five pieces** and implements standard queue operations:

| Operation   | Purpose                                      |
| ----------- | -------------------------------------------- |
| `enqueue()` | Add a piece to the rear                      |
| `dequeue()` | Remove the piece at the front                |
| `peek()`    | Inspect the front piece                      |
| `isEmpty()` | Check whether the queue is empty             |
| `isFull()`  | Check whether the queue has reached capacity |
| `getSize()` | Return the current number of pieces          |
| `clear()`   | Empty the queue                              |

The queue can also expose its contents for **undo-state restoration** and upcoming-piece display.

---

### 3. PieceQueue — Piece Generation

`PieceQueue` acts as the layer connecting the **piece bag** and the **upcoming-piece queue**.

```text
                 PieceQueue
                     │
             ┌───────┴───────┐
             ▼               ▼
          Queue          LinkedList
             │               │
             ▼               ▼
       Next pieces        Piece bag
```

It is responsible for:

* Filling the initial queue
* Randomly selecting pieces from the bag
* Supplying the next piece to the game
* Providing upcoming pieces for the interface
* Saving the current queue state
* Restoring the queue after an undo

This combines multiple data structures into one gameplay system rather than using them independently.

---

### 4. Linked List — Game Board

The game board uses a separate linked-list structure to represent its rows.

Each `RowNode` contains an array of **15 cells** and a pointer to the next row.

```text
Board
 │
 ▼
Row 0 ──► Row 1 ──► Row 2 ──► ... ──► Row 19
 │          │          │                 │
15 cells  15 cells   15 cells          15 cells
```

The board contains:

* **20 rows**
* **15 columns**
* A linked list of `RowNode` objects

The board implementation provides operations for:

* Adding rows
* Accessing rows
* Detecting completed rows
* Clearing rows
* Checking cell occupancy
* Detecting collisions
* Saving the board state
* Restoring the board state

---

### 5. Stack — Undo System

The undo system follows the **LIFO (Last In, First Out)** principle.

Previous game states are pushed onto a custom stack as the game progresses.

```text
        UndoStack
           │
           ▼
     ┌───────────┐
     │ State 3   │  ← most recent
     ├───────────┤
     │ State 2   │
     ├───────────┤
     │ State 1   │
     └───────────┘
```

When the player performs an undo operation, the most recent saved state is restored first.

Game-state information can include elements such as:

* Board state
* Score
* Current piece
* Upcoming pieces

This allows the game to return to an earlier state rather than simply reversing one individual movement.

---

### 6. AVL Tree — Leaderboard

The leaderboard uses a custom **AVL tree** to organise scores.

An AVL tree is a self-balancing binary search tree. After insertions, the tree performs rotations when necessary to maintain its height balance.

```text
              Score
             /     \
          Score    Score
           /          \
        Score        Score
```

The AVL implementation supports:

* Inserting scores
* Maintaining tree balance
* Retrieving high scores
* Managing leaderboard data
* Clearing the tree

Using an AVL tree provides an ordered and balanced structure for score management rather than storing leaderboard entries in an unsorted collection.

---

## 🏗️ Game Architecture

The project is organised into separate layers for gameplay, data structures, leaderboard management, and the user interface.

```text
                         ┌──────────────┐
                         │   main.cpp   │
                         └──────┬───────┘
                                │
              ┌─────────────────┼─────────────────┐
              │                 │                 │
              ▼                 ▼                 ▼
          Game Logic       Data Structures     UI / Screens
              │                 │                 │
              │          ┌──────┼──────┐          │
              │          │      │      │          │
              │          ▼      ▼      ▼          │
              │       Linked  Queue  Stack        │
              │       List           /Undo        │
              │          │                         │
              │          ▼                         │
              │       AVL Tree                     │
              │          │                         │
              └──────────┼─────────────────────────┘
                         ▼
                    Raylib Layer
```

### Main Components

| Directory          | Responsibility                                        |
| ------------------ | ----------------------------------------------------- |
| `game/`            | Core Tetris gameplay and piece logic                  |
| `data_structures/` | Custom DSA implementations                            |
| `leaderboard/`     | Score and leaderboard management                      |
| `ui/`              | Screens, interface elements, colours and presentation |
| `assets/`          | Images, audio and fonts                               |

---

## 📁 Project Structure

```text
DSA-Project/
│
├── assets/
│   ├── audio/
│   ├── fonts/
│   └── images/
│
├── include/
│   ├── data_structures/
│   │   ├── LinkedList.h
│   │   ├── PieceQueue.h
│   │   ├── Queue.h
│   │   ├── ScoreAVL.h
│   │   └── UndoStack.h
│   │
│   ├── game/
│   │   ├── Board.h
│   │   ├── Game.h
│   │   ├── Piece.h
│   │   ├── Pieces.h
│   │   └── Position.h
│   │
│   ├── leaderboard/
│   │   └── Leaderboard.h
│   │
│   └── ui/
│       ├── Colours.h
│       ├── Manager.h
│       └── WelcomeScreen.h
│
├── src/
│   ├── data_structures/
│   ├── game/
│   ├── leaderboard/
│   ├── ui/
│   └── main.cpp
│
├── .gitignore
├── Makefile
└── README.md
```

---

## 🎮 Gameplay Systems

### Board

The `Board` class manages the game grid, including:

* Cell occupancy
* Collision detection
* Piece locking
* Completed-row detection
* Row clearing
* Board-state saving and restoration

### Pieces

The project implements the seven standard Tetris pieces:

```text
I   O   T   L   J   S   Z
```

Each piece derives from the base `Piece` class and has its own shape and behaviour.

### Game

The `Game` class coordinates the main gameplay systems, including:

* Board management
* Piece movement
* Rotation
* Scoring
* Piece generation
* Undo operations
* Game state

---

## 🖥️ Technologies

| Technology             | Role                                          |
| ---------------------- | --------------------------------------------- |
| **C++17**              | Application and data-structure implementation |
| **Raylib 5.5**         | Graphics, audio, input and window management  |
| **MinGW-w64 / GCC**    | C++ compilation                               |
| **GNU Make**           | Build automation                              |
| **Visual Studio Code** | Development environment                       |

---

## ⚙️ Build & Run

### Requirements

* Windows
* C++17-compatible compiler
* MinGW-w64 / GCC
* GNU Make
* Raylib 5.5

### Build

Open PowerShell in the project directory:

```powershell
mingw32-make
```

### Run

```powershell
.\main.exe
```

### Clean

To remove the generated executable:

```powershell
mingw32-make clean
```

The project uses the included `Makefile` to compile all source files with **C++17** and link them against Raylib and the required Windows libraries.

---

## 🔍 Why Data Structures Matter Here

The data structures in this project are not included merely to demonstrate their syntax. Each one solves a specific problem within the game.

| Structure               | Game System      | Why It Fits                                  |
| ----------------------- | ---------------- | -------------------------------------------- |
| **Linked List**         | Piece bag        | Dynamic sequence of available pieces         |
| **Linked List**         | Board rows       | Sequential row representation                |
| **Queue**               | Upcoming pieces  | FIFO ordering                                |
| **Stack**               | Undo             | Most recent state must be restored first     |
| **AVL Tree**            | Leaderboard      | Maintains ordered, balanced score data       |
| **Combined structures** | Piece generation | Coordinates the piece bag and upcoming queue |

This integration is the main focus of the project: **applying data-structure concepts to real game functionality.**

---

<div align="center">

### 🎮 Built as a practical application of Data Structures & Algorithms

**C++17 • Raylib • Custom Data Structures**

</div>
