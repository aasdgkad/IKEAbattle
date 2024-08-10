# Compiler
CXX = g++

# Project Name
TARGET = ikea

# Directories
INCLUDE_DIR = D:\ikeaBattle\SFML-2.6.1\include
LIB_DIR = D:\ikeaBattle\SFML-2.6.1\lib

# Source Files
SRC = main.cpp Player.cpp CutScene.cpp map.cpp Boss.cpp Collidable.cpp

# Object Files
OBJ = $(SRC:.cpp=.o)

# Compiler Flags
CXXFLAGS = -I$(INCLUDE_DIR) -DSFML_STATIC

# Linker Flags
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lopengl32 -lgdi32 -lwinmm -static

# Default rule
all: $(TARGET) run

# Build the target
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Compile rule for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable after building
run:
	./$(TARGET).exe

# Clean rule to remove the executable and object files
clean:
	del $(TARGET).exe $(OBJ)

.PHONY: all clean run