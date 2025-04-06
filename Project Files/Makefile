CC=gcc
CFLAGS=-I.
DEPS = alias.h built_inCommands.h
OBJ = newshell.o alias.o built_inCommands.o

# Rule to compile object files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Target that links the object files into the final executable
newshell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o newshell
