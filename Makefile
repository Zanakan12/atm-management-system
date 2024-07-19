CC = gcc
CFLAGS = -Isrc -Wall
LDFLAGS =

# Dossier contenant les fichiers source
SRC_DIR = src

# Dossier pour les fichiers objets
OBJ_DIR = obj

# Les fichiers source
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Correspondance des fichiers objets
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Nom de l'exécutable
EXEC = atm

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXEC)

.PHONY: all clean
