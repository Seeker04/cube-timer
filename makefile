COMP    = gcc
FLAGS   = -W -Wall -Wextra -Wconversion -Werror -pedantic -std=c89
SOURCES = main.c
BIN     = ctimer

main: $(SOURCES)
	$(COMP) $(FLAGS) -o $(BIN) $(SOURCES)

clean:
	rm -f $(BIN)

