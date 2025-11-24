#include <iostream>
#include "Game.h"
#include <random>
#include <vector>
using namespace std;

Game::Game() {
	// Initialize piece bag using LinkedList
	pieceBag.AddPiece(IPiece());
	pieceBag.AddPiece(JPiece());
	pieceBag.AddPiece(LPiece());
	pieceBag.AddPiece(OPiece());
	pieceBag.AddPiece(SPiece());
	pieceBag.AddPiece(TPiece());
	pieceBag.AddPiece(ZPiece());

	current = GetRandomPiece();
	next = GetRandomPiece();
	GameOver = false;
	score = 0;  // KEEP: Your variable name
	InitAudioDevice();
	music = LoadMusicStream("Sounds/music.mp3");
	musicOn = true;
	PlayMusicStream(music);
	RotateSound = LoadSound("Sounds/rotate.mp3");
	ClearSound = LoadSound("Sounds/clear.mp3");

	// YOUR: Initialize your features
	isCountingDown = true;
	countdownNumber = 3;
	countdownStartTime = GetTime();
	ghostPiece = current.GetGhostPiece(board);
	showGhost = true;
	isDropping = false;
	gameStartTime = 0;
	totalPlayTime = 0;
	isTimeTracking = false;
	lastPauseTime = 0;

	undoStack = UndoStack(50);        // 50 undo steps
	pieceQueue = PieceQueue(5);       // 5-piece preview

	// Initialize piece queue with starting pieces
	pieceQueue.FillStartup();
	current = pieceQueue.Dequeue();
	next = pieceQueue.Peek();

}

double Game::GetPlayTime() const {
	if (!isTimeTracking) return totalPlayTime;
	return totalPlayTime + (GetTime() - gameStartTime);
}

void Game::StartTimeTracking() {
	if (!isTimeTracking) {
		gameStartTime = GetTime();
		isTimeTracking = true;
	}
}

void Game::StopTimeTracking() {
	if (isTimeTracking) {
		totalPlayTime += (GetTime() - gameStartTime);
		isTimeTracking = false;
	}
}

void Game::StartCountdown() {
	isCountingDown = true;
	countdownNumber = 3;
	countdownStartTime = GetTime();
	StopTimeTracking();
}

void Game::UpdateCountdown() {
	if (!isCountingDown) return;

	double currentTime = GetTime();
	double elapsed = currentTime - countdownStartTime;

	if (elapsed >= 1.0 && countdownNumber > 1) {
		countdownNumber--;
		countdownStartTime = currentTime;
	}
	else if (elapsed >= 1.0 && countdownNumber == 1) {
		isCountingDown = false;
		countdownNumber = 0;
		StartTimeTracking();
	}

}


Game::~Game() {
	UnloadSound(RotateSound);
	UnloadSound(ClearSound);
	UnloadMusicStream(music);
	CloseAudioDevice();
}

void Game::ToggleMusic() {
	musicOn = !musicOn;
	if (musicOn) {
		ResumeMusicStream(music);
	}
	else {
		PauseMusicStream(music);
	}
}

Piece Game::GetRandomPiece() {
	if (pieceBag.GetSize() == 0) {
		// Refill the bag if empty (Tetris randomizer technique)
		pieceBag.AddPiece(IPiece());
		pieceBag.AddPiece(JPiece());
		pieceBag.AddPiece(LPiece());
		pieceBag.AddPiece(OPiece());
		pieceBag.AddPiece(SPiece());
		pieceBag.AddPiece(TPiece());
		pieceBag.AddPiece(ZPiece());
	}

	int index = rand() % pieceBag.GetSize();
	Piece p = pieceBag.GetPiece(index);
	pieceBag.RemovePiece(index);  // Remove from bag once used
	return p;
}

void Game::UpdateGhostPiece() {
	ghostPiece = current.GetGhostPiece(board);
}

void Game::Draw() {
	board.Draw();
	if (showGhost) {
		ghostPiece.DrawGhost(11, 11);
	}
	current.Draw(11, 11);
	switch (next.id) {
	case 3:
		next.Draw(256, 670);  // KEEP: Your coordinates
		break;
	case 4:
		next.Draw(255, 660);  // KEEP: Your coordinates
		break;
	default:
		next.Draw(260, 670);  // KEEP: Your coordinates
		break;
	}
}

void Game::MoveLeft() {
	if (!GameOver) {
		current.Move(0, -1);
		if (HasCollided() || PieceFits() == false) {
			current.Move(0, 1);
		}
	}
}
void Game::MoveRight() {
	if (!GameOver) {
		current.Move(0, 1);
		if (HasCollided() || PieceFits() == false) {
			current.Move(0, -1);
		}
	}
}

void Game::MoveDown() {
	if (!GameOver) {
		current.Move(1, 0);
		if (HasCollided() || PieceFits() == false) {
			current.Move(-1, 0);
			LockPiece();
		}
		UpdateGhostPiece();  // YOUR: Update ghost after moving down
	}
}


void Game::HandleInput() {
	int key = GetKeyPressed();
	if (GameOver && key != 0) {
		GameOver = false;
		Reset();
	}

	bool pieceMoved = false;

	switch (key) {
	case KEY_LEFT:
		SaveUndoState();
		MoveLeft();
		pieceMoved = true;
		break;
	case KEY_RIGHT:
		SaveUndoState();
		MoveRight();
		pieceMoved = true;
		break;
	case KEY_DOWN:
		SaveUndoState();
		MoveDown();
		UpdateScore(0, 1);
		pieceMoved = true;
		break;
	case KEY_UP:
		SaveUndoState();
		RotatePiece();
		pieceMoved = true;
		break;
	case KEY_SPACE:  // YOUR: Hard drop
		SaveUndoState();
		HardDrop();
		pieceMoved = true;
		break;
	case KEY_Z:  // ADD UNDO KEY (Z key)
		if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
			UndoMove();
			pieceMoved = true;
		}
		break;
	}

	// YOUR: Update ghost piece if the current piece moved or rotated
	if (pieceMoved && showGhost) {
		UpdateGhostPiece();
	}
}

void Game::HardDrop() {
	if (!GameOver && !isCountingDown && !isDropping) {
		isDropping = true;
	}
}

void Game::UpdateHardDrop() {
	if (!isDropping) return;

	current.Move(1, 0);
	if (HasCollided() || !PieceFits()) {
		current.Move(-1, 0);
		LockPiece();
		isDropping = false;
		return;
	}
	score += 2;  // KEEP: Your scoring
}

bool Game::HasCollided() {
	vector<Position> tiles = current.GetCellPositions();
	for (Position item : tiles) {
		if (board.CollisionDetected(item.ROW, item.COL)) {
			return true;
		}
	}
	return false;
}

void Game::RotatePiece() {
	if (!GameOver) {
		bool rotationSuccess = current.RotateWithWallKicks(board);
		if (rotationSuccess) {
			PlaySound(RotateSound);
			UpdateGhostPiece();  // YOUR: Update ghost after rotation
		}
	}
}


void Game::LockPiece() {
	vector <Position> tiles = current.GetCellPositions();
	for (Position item : tiles) {
		board.SetCell(item.ROW, item.COL, current.id);
	}

	// USE PIECE QUEUE INSTEAD OF RANDOM
	current = pieceQueue.Dequeue();
	next = pieceQueue.Peek();
	undoStack.Clear();  // Clear undo history after locking

	if (PieceFits() == false) {
		GameOver = true;
	}

	int rowsCleared = board.ClearRows();
	if (rowsCleared > 0) {
		PlaySound(ClearSound);
		UpdateScore(rowsCleared, 0);
	}
	UpdateGhostPiece();
}

bool Game::PieceFits() {
	vector<Position> tiles = current.GetCellPositions();
	for (Position item : tiles) {
		if (board.isCellEmpty(item.ROW, item.COL) == false) {
			return false;
		}
	}
	return true;
}


void Game::Reset() {
	board.Initialize();

	// USE PIECE QUEUE INSTEAD OF LINKED LIST BAG
	pieceQueue.Clear();
	pieceQueue.FillStartup();
	current = pieceQueue.Dequeue();
	next = pieceQueue.Peek();

	undoStack.Clear();  // CLEAR UNDO STACK

	GameOver = false;
	score = 0;
	UpdateGhostPiece();
	totalPlayTime = 0;
	isTimeTracking = false;
	gameStartTime = 0;
	totalLinesCleared = 0;
}


void Game::UpdateScore(int lines, int down) {
	if (lines > 0) {
		totalLinesCleared += lines;
	}
	if (lines == 1)
		score += 100;
	else if (lines == 2)
		score += 200;
	else if (lines == 3)
		score += 500;
	else if (lines == 4)
		score += 800;
	else if (lines > 4) {
		lines -= 4;
		lines *= 100;
		score += 800 + lines;
	}

	score += down;

	// ADD: Insert into AVL tree
	scores.Insert(score);
}

// UNDO SYSTEM FUNCTIONS
void Game::SaveUndoState() {
	undoStack.Push(current);
}

void Game::UndoMove() {
	if (!undoStack.IsEmpty() && !GameOver) {
		current = undoStack.Pop();
		UpdateGhostPiece();
	}
}
