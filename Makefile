BIN = ./bin
DATA = ./data
INCLUDE = ./include
OBJ = ./obj
SRC = ./src


all: libed app

libed: \
	$(OBJ)/crud.o \
	$(OBJ)/hash.o \
	$(OBJ)/rotins.o \
	


app:
	gcc $(SRC)/main.c $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/app

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	gcc -c $< -I $(INCLUDE) -o $@


clear:
	rm -rf $(BIN)/*
	rm -rf $(OBJ)/*

run:
	$(BIN)/app