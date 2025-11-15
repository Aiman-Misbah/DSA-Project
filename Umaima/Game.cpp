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
	score = 0;
	InitAudioDevice();
	music = LoadMusicStream("Sounds/music.mp3");
	PlayMusicStream(music);
	RotateSound = LoadSound("Sounds/rotate.mp3");
	ClearSound = LoadSound("Sounds/clear.mp3");
}

Game::~Game(){
	UnloadSound(RotateSound);
	UnloadSound(ClearSound);
	UnloadMusicStream(music);
	CloseAudioDevice(); 
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

void Game::Draw() {
	board.Draw();
	current.Draw(11,11);
	switch (next.id) {
	case 3:
		next.Draw(256, 270);
		break;
	case 4:
		next.Draw(255, 260);
		break;
	default:
		next.Draw(260, 270);
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
	}
}


void Game::HandleInput() {
	int key = GetKeyPressed();
	if (GameOver && key != 0) {
		GameOver = false;
		Reset();
	}
	switch (key) {
	case KEY_LEFT:
		MoveLeft();
		break;
	case KEY_RIGHT:
		MoveRight();
		break;
	case KEY_DOWN:
		MoveDown();
		UpdateScore(0, 1); 
		break;
	case KEY_UP:
		RotatePiece();
		break;
	}
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
		// ?? CHANGED: Use recursive wall kick rotation
		bool rotationSuccess = current.RotateWithWallKicks(board);
		
		if (rotationSuccess) {
			PlaySound(RotateSound);
		}
		// If rotation fails, the recursive method already backtracked
		// No need to call UndoRotation() manually
	}
}

void Game::LockPiece() {
	vector <Position> tiles = current.GetCellPositions();
	for (Position item : tiles) {
		board.SetCell(item.ROW, item.COL, current.id);
	}
	current = next;
	if (PieceFits() == false) {
		GameOver = true;
	}
	next = GetRandomPiece();
	int rowsCleared = board.ClearRows();
	if (rowsCleared > 0) {
		PlaySound(ClearSound);
		UpdateScore(rowsCleared, 0);
	}
}

bool Game::PieceFits() {
	vector<Position> tiles = current.GetCellPositions();
	for (Position item : tiles) {
		if (board.isCellEmpty(item.ROW, item.COL)==false) {
			return false;
		}
	}
	return true;
}


void Game::Reset() {
	board.Initialize();

	// Refill the piece bag
	pieceBag.Clear();
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
	score = 0;
}


void Game::UpdateScore(int lines, int down) {
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
}