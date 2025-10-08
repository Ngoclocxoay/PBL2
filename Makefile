# ===== Windows (MinGW / w64devkit) =====
CXX := D:/raylib/w64devkit/bin/g++.exe

# Thư mục raylib local của anh (chứa src/, trong đó có libraylib.a sau khi build)
RAYLIB_DIR := D:/raylib/raylib
RAYLIB_INC := $(RAYLIB_DIR)/src
RAYLIB_LIB := $(RAYLIB_DIR)/src

TARGET  := chess.exe
SOURCES := src/main.cpp src/Game.cpp src/Board.cpp src/Piece.cpp src/TextureCache.cpp src/library.cpp src/King.cpp src/Queen.cpp src/Bishop.cpp src/Knight.cpp src/Rook.cpp src/Pawn.cpp
OBJECTS := $(SOURCES:.cpp=.o)

CXXFLAGS := -std=c++17 -Wall -Wextra -I$(RAYLIB_INC) -Isrc -I.

# Linker flags cho raylib trên Windows
LDFLAGS := -L$(RAYLIB_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32 -lole32 -luuid \
           -static -static-libgcc -static-libstdc++

# Build
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET) *.o

.PHONY: run clean
