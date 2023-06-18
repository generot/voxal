CC=g++
CFLAGS=-Wall -pedantic

BIN=./bin
SRC=./src

INCPATH=./include

OBJ=$(patsubst %.cc, %.o, $(wildcard $(SRC)/*.cc))

$(BIN)/voxal.exe: $(OBJ)
	$(CC) -o $@ $^

$(SRC)/%.o: $(SRC)/%.cc $(INCPATH)/%.h
	$(CC) -o $@ -c $< -I $(INCPATH)

.PHONY: clean rmobj

clean:
	rm $(SRC)/*.o
	rm $(BIN)/*.exe

rmobj:
	rm $(SRC)/*.o