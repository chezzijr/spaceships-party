.SILENT: all run

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
INCLUDE_PATHS = ./include ./src
COMPILER_FLAGS = -w $(foreach d, $(INCLUDE_PATHS), -I$d)

#LINKER_FLAGS specifies the libraries we're linking against
# link against the SDL2 library and the SDL2_image library, libjxl
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

#OBJ_NAME specifies the name of our executable
OBJ_NAME = game
OBJ_DIR = ./dist
OUTPUT = $(OBJ_DIR)/$(OBJ_NAME)

#This is the target that compiles our executable
all:
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	$(CC) -g $(shell find ./src -type f -iregex ".*\.cpp") -o $(OUTPUT) $(COMPILER_FLAGS) $(LINKER_FLAGS)
run:
	./dist/$(OBJ_NAME)