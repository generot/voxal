CC=g++
CFLAGS=-Wall -pedantic -g

BIN=./bin
SRC=./src

INCPATH=./include

OBJ=$(patsubst %.cc, %.o, $(wildcard $(SRC)/*.cc))

$(BIN)/voxal.exe: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC)/%.o: $(SRC)/%.cc $(INCPATH)/%.h
	$(CC) $(CFLAGS) -o $@ -c $< -I $(INCPATH)

.PHONY: clean rmobj

clean:
	rm $(SRC)/*.o
	rm $(BIN)/*.exe

rmobj:
	rm $(SRC)/*.o