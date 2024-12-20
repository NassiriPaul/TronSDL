# Nom de l'exécutable
EXEC = game

# Répertoires
SRCDIR = src
INCDIR = include
BINDIR = bin
OBJDIR = obj

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -I$(INCDIR) -O2

# Fichiers sources par module
MODEL_SRC = $(SRCDIR)/model/grid.c \
            $(SRCDIR)/model/rider.c \
            $(SRCDIR)/model/route.c
# MODEL_SRC += $(SRCDIR)/model/model.c

VIEW_SRC = $(SRCDIR)/view/ncurses_view.c

CONTROLLER_SRC = $(SRCDIR)/controller/game_controller.c
#\
				$(SRCDIR)/controller/input_handler.c 

MAIN_SRC = $(SRCDIR)/main.c

# Tous les sources
SRC = $(MODEL_SRC) $(VIEW_SRC) $(CONTROLLER_SRC) $(MAIN_SRC)# $(MAIN_SRC)

# Génération des objets : on transforme chaque .c en .o
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# Cible par défaut
all: $(EXEC)

# Règle pour créer l'exécutable
$(EXEC): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$(EXEC) -lncurses
	@echo "Compilation terminée. L'exécutable est dans $(BINDIR)/$(EXEC)"

# Règle générique pour compiler chaque .c en .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des objets et de l'exécutable
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Nettoyage complet
mrproper: clean
	rm -f *~ \#*\#

# Pour relancer une compilation propre
re: mrproper all
