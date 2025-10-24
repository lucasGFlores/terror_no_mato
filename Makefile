# === TARGET & DIRECTORIES ===
TARGET = game
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# === Compiler & Flags ===
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(INC_DIR)

# Add raylib include path (Homebrew default)
RAYLIB_PREFIX := $(shell brew --prefix raylib)
CXXFLAGS += -I$(RAYLIB_PREFIX)/include

# === Libraries for macOS ===
LIBS = -L$(RAYLIB_PREFIX)/lib -lraylib -lm -lpthread \
       -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# === Source / Object Files ===
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# === Default target ===
all: $(TARGET)

# === Compile .cpp → .o ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Link objects into executable ===
$(TARGET): $(OBJS)
	@echo "Linking objects into $(TARGET) ..."
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# === Ensure obj folder exists ===
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# === Clean everything ===
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# === Rebuild from scratch ===
rebuild: clean all

.PHONY: all clean rebuild
