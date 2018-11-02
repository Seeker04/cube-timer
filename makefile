COMP    = gcc
FLAGS   = -W -Wall -Wextra -Wconversion -pedantic -std=c89 -Werror
SOURCES = main.c
BIN     = ctimer

main: $(SOURCES)
	$(COMP) $(FLAGS) -o $(BIN) $(SOURCES)

clean:
	rm -f $(BIN)

