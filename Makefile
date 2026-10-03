CXX = C:/xpack-mingw-w64-gcc-12.2.0-1-win32-x64/xpack-mingw-w64-gcc-12.2.0-1/bin/g++.exe

VCPKG = C:/vcpkg/installed/x64-mingw-dynamic

INCLUDES = -I$(VCPKG)/include -Iinclude
LIBPATH  = -L$(VCPKG)/lib
LIBS     = -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = src/main.cpp \
      src/data_structures/LinkedList.cpp \
      src/data_structures/Queue.cpp \
      src/data_structures/ScoreAVL.cpp \
      src/data_structures/UndoStack.cpp \
      src/game/Board.cpp \
      src/game/Game.cpp \
      src/game/Piece.cpp \
      src/game/PieceQueue.cpp \
      src/game/Pieces.cpp \
      src/game/Position.cpp \
      src/leaderboard/Leaderboard.cpp \
      src/ui/Colours.cpp \
      src/ui/Manager.cpp \
      src/ui/WelcomeScreen.cpp

OUT = main.exe

all:
	$(CXX) -g -std=c++17 $(SRC) -o $(OUT) $(INCLUDES) $(LIBPATH) $(LIBS)

clean:
	del $(OUT)
