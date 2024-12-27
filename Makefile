############################################################
# Makefile: Single choice (UI=...) for both controller & view
############################################################

# If UI is not specified on the command line, default to ncurses.
UI ?= ncurses

EXEC       = game
SRCDIR     = src
INCDIR     = include
BINDIR     = bin
OBJDIR     = obj
TESTDIR    = tests

CC         = gcc
CFLAGS     = -ansi -pedantic -I$(INCDIR)
LDLIBS     =

############################################################
# 1) Choose (controller + view) based on UI
############################################################
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

############################################################
# 2) Model + Common controller code + main
############################################################
MODEL_SRC      = $(SRCDIR)/model/grid.c \
                 $(SRCDIR)/model/rider.c \
                 $(SRCDIR)/model/route.c

COMMON_CTRL_SRC= $(SRCDIR)/controller/game_controller.c

MAIN_SRC       = $(SRCDIR)/main.c

############################################################
# 3) Combine all sources
############################################################
SRC = $(MODEL_SRC) $(COMMON_CTRL_SRC) $(CTR_SRC) $(VIEW_SRC) $(MAIN_SRC)

# Transform .c -> .o in OBJDIR
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

############################################################
# 4) Build the game (default target)
############################################################
all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDLIBS)
	@echo "Built $(UI) version -> $@"

############################################################
# 5) Generic rule for .c -> .o
############################################################
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

############################################################
# 6) Optional test target
############################################################
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

############################################################
# 7) Cleaning
############################################################
clean:
	rm -rf $(OBJDIR) $(BINDIR)

mrproper: clean
	rm -f *~ \#*\#

re: mrproper all
