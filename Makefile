CC = gcc

SRC = main.c knapsack.c heuristique.c utils.c
OBJ = $(SRC:.c=.o)
EXEC = sadm_solver

CFLAGS = -Wall -Wextra -O2 -fpermissive

# Règle par défaut
all: $(EXEC)

# Règle pour créer l'exécutable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Règle pour compiler les fichiers .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	del $(OBJ) $(EXEC)
	
# Nettoyage des fichiers générés + exe
cleanf: clean 
	del *.exe
	 
# Nettoyage complet (incluant les fichiers temporaires)
distclean: clean
	del *~ .*~

# Test de l'exécution
run: $(EXEC)
	$(EXEC) .\Instances_MKP\100M5_1.txt 4
run2: $(EXEC)
	$(EXEC) .\Instances_MKP\250M5_21.txt 4