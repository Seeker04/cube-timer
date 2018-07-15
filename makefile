COMP = gcc
FLAGS = -W -Wall -Wextra -pedantic -std=c99
SOURCES = *.c
BIN = ctimer

main: $(SOURCES)
	$(COMP) $(FLAGS) -o $(BIN) $(SOURCES)

clean:
	rm -f $(BIN)
