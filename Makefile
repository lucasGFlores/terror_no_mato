TARGET = game

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

#Flags to compile
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I$(INC_DIR)

# links to compile in the project like: raylib, thread, OpenGL...
LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11

#Procura todos os arquivos .cpp em src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

#Converte os nomes dos arquivos .cpp em .o (fazendo uma lista de nomes)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

#compiling .cpp to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "OBJS: $(OBJS)"
	@echo "compiling $< ..."
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
#Generating the executable file
$(TARGET): $(OBJS)
	@echo "SRCS: echo "SRC_DIR=[$(SRC_DIR)]""
	@echo "Linking the .o arquivos to $(TARGET)"
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Garante que a pasta obj existe
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Comando pra limpar tudo
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Comando pra recompilar do zero
rebuild: clean all
