ifeq ($(UI),)
  $(error "You must specify UI=ncurses or UI=sdl on the command line. E.g.: make UI=ncurses")
endif

EXEC       = game
SRCDIR     = src
INCDIR     = include
BINDIR     = bin
OBJDIR     = obj
TESTDIR    = tests

CC         = gcc
CFLAGS     = -ansi -pedantic -I$(INCDIR)
LDLIBS     =

ifeq ($(UI),ncurses)
  CTR_SRC  = $(SRCDIR)/controller/ncurses_controller.c
  VIEW_SRC = $(SRCDIR)/view/ncurses_view.c
  LDLIBS   += -lncurses
else ifeq ($(UI),sdl)
  CTR_SRC  = $(SRCDIR)/controller/sdl_controller.c
  VIEW_SRC = $(SRCDIR)/view/sdl_view.c
  LDLIBS   += -lSDL2 -lSDL2_ttf
else
  $(error Unknown UI type "$(UI)"! Use `make UI=ncurses` or `make UI=sdl`)
endif

# Model + Common controller code + main
MODEL_SRC      = $(SRCDIR)/model/grid.c \
                 $(SRCDIR)/model/rider.c \
                 $(SRCDIR)/model/route.c

COMMON_CTRL_SRC= $(SRCDIR)/controller/game_controller.c

MAIN_SRC       = $(SRCDIR)/main.c

# Combine all sources
SRC = $(MODEL_SRC) $(COMMON_CTRL_SRC) $(CTR_SRC) $(VIEW_SRC) $(MAIN_SRC)

# Transform .c -> .o in OBJDIR
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# Build the game (default target)
all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDLIBS)
	@echo "Built $(UI) version -> $@"

# .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Optional test target
TEST_SOURCES = $(TESTDIR)/test_grid.c
TEST_OBJ     = $(patsubst $(TESTDIR)/%.c, $(OBJDIR)/tests/%.o, $(TEST_SOURCES))
TEST_EXEC    = $(BINDIR)/test_exec

$(TEST_EXEC): $(OBJ) $(TEST_OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	@echo "Test executable -> $@"

$(OBJDIR)/tests/%.o: $(TESTDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Cleaning
clean:
	rm -rf $(OBJDIR) $(BINDIR)

mrproper: clean
	rm -f *~ \#*\#

re: mrproper all
