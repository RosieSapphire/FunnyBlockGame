CC=cc
CFLAGS=-Wall -Wextra -Og -g3
INC=-Iinclude -Ideps/rmath/include -Ideps/assimp/include \
    -Ideps/assimp/build/include
LIB=$(shell pkg-config --libs glfw3) -lm -Ldeps/rmath -lrmath \
    -Ldeps/assimp/build/bin -lassimp
SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c, %.o, $(SRC))
BIN=funny_block_game

default: $(BIN)

$(BIN): $(OBJ)
	make -C deps/rmath/
	$(CC) $(CFLAGS) $^ -o $@ $(INC) $(LIB)
	rm -f *.o
	@echo Success!

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ $(INC)

clean:
	@rm -f $(BIN) $(OBJ)
	@echo Cleaned up build files.
