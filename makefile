CC=g++
CFLAGS=-Wall -pedantic

BIN=./bin
SRC=./src

INCPATH=./include

OBJ=$(patsubst %.cc, %.o, $(wildcard $(SRC)/*.cc))
INC=$(wildcard $(INCPATH)/*.h)

$(BIN)/voxal.exe: $(OBJ)
	$(CC) -o $@ $^

$(SRC)/%.o: $(SRC)/%.cc $(INC)
	$(CC) -o $@ -c $< -I $(INCPATH)

.PHONY: clean rmobj

clean:
	rm $(SRC)/*.o
	rm $(BIN)/*.exe

rmobj:
	rm $(SRC)/*.o