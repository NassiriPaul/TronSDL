EXEC       = game
SRCDIR     = src
INCDIR     = include
BINDIR     = bin
OBJDIR     = obj
TESTDIR    = tests

CC         = gcc
CFLAGS     =  -ansi -pedantic -I$(INCDIR)

LIB_SRC = $(SRCDIR)/model/grid.c \
          $(SRCDIR)/model/rider.c \
          $(SRCDIR)/model/route.c \
          $(SRCDIR)/controller/game_controller.c \
          $(SRCDIR)/controller/ncurses_controller.c \
          $(SRCDIR)/view/ncurses_view.c

LIB_OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(LIB_SRC))

GAME_MAIN = $(SRCDIR)/main.c
GAME_MAIN_OBJ = $(OBJDIR)/main.o

GAME_EXEC = $(BINDIR)/game

TEST_SOURCES = $(TESTDIR)/test_grid.c
TEST_OBJ = $(patsubst $(TESTDIR)/%.c, $(OBJDIR)/tests/%.o, $(TEST_SOURCES))
TEST_EXEC = $(BINDIR)/test_exec

# ------------------------------------------------------------------
all: $(GAME_EXEC)

$(GAME_EXEC): $(LIB_OBJ) $(GAME_MAIN_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses
	@echo "Game built -> $@"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(GAME_MAIN_OBJ): $(GAME_MAIN)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------------
# Tests
# ------------------------------------------------------------------

$(TEST_EXEC): $(LIB_OBJ) $(TEST_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses
	@echo "Test executable built -> $@"

$(OBJDIR)/tests/%.o: $(TESTDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# ------------------------------------------------------------------
# Cleaning
# ------------------------------------------------------------------
clean:
	rm -rf $(OBJDIR) $(BINDIR)

mrproper: clean
	rm -f *~ \#*\#

re: mrproper all
