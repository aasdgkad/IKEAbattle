# Compiler
CXX = g++

# Project Name
TARGET = ikea

# Directories
INCLUDE_DIR = D:\ikeaBattle\SFML-2.6.1\include
LIB_DIR = D:\ikeaBattle\SFML-2.6.1\lib

# Source Files
SRC = main.cpp

# Compiler Flags
CXXFLAGS = -I$(INCLUDE_DIR) -DSFML_STATIC

# Linker Flags
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-audio-s -lsfml-network-s -lopengl32 -lgdi32 -lwinmm -static

# Default rule
all: $(TARGET) run

# Build the target
$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Run the executable after building
run:
	./$(TARGET).exe

# Clean rule to remove the executable
clean:
	del $(TARGET).exe

.PHONY: all clean run
