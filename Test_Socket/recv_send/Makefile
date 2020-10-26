.PHONY:clean all
CC=gcc
CFLAGS=-w -g
BIN=echosrv echocli getip 
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)
