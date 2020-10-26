.PHONY:clean all
CC=gcc
CFLAGS=-w -g
BIN=echosrv echocli echosrv2 echocli2
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
