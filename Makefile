CC      = gcc
CFLAGS  = -std=c11 -Wall -fsanitize=address -g $(shell pkg-config --cflags criterion 2>/dev/null)
LDLIBS  = -fsanitize=address $(shell pkg-config --libs criterion 2>/dev/null)

SRC      = src/arena.c
TEST_SRC = test/test_arena.c

test_arena: $(SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: test clean
test: test_arena
	./test_arena

clean:
	rm -f test_arena
	rm -rf *.dSYM

# -------------------------------------------------------------------
# Editor support: generate compile_commands.json for clangd
# -------------------------------------------------------------------

BEAR := $(shell command -v bear 2>/dev/null)

.PHONY: compile_commands.json
compile_commands.json: clean
ifndef BEAR
	$(error "bear is not installed. Install it with: brew install bear")
endif
	$(BEAR) -- make test_arena
