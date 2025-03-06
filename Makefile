CC = gcc

SRC = knapsack.c heuristique.c genetic.c chrono.c
OBJ = $(SRC:.c=.o)
EXEC = sadm_solver
BENCH_EXEC = sadm_bench

CFLAGS = -Wall -Wextra -O2

# Règle par défaut
all: $(EXEC)

# Règle pour créer l'exécutable principal
$(EXEC): main.o $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) main.o $(OBJ)

# Règle pour créer l'exécutable de benchmark
$(BENCH_EXEC): benchmark.o $(OBJ)
	$(CC) $(CFLAGS) -o $(BENCH_EXEC) benchmark.o $(OBJ)

# Règle pour compiler les fichiers .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers générés
clean:
	del $(OBJ) $(EXEC) $(BENCH_EXEC) main.o benchmark.o

# linux
cleanl:
	rm $(OBJ) $(EXEC) $(BENCH_EXEC) main.o benchmark.o
	
# Nettoyage des fichiers générés + exe
cleanf: clean 
	del *.exe

# linux
cleanfl: cleanl
	rm *.exe
	 
# Nettoyage complet (incluant les fichiers temporaires)
distclean: cleanf
	del *~ .*~

distcleanl: cleanfl
	rm -rf *~ .*~

# Test de l'exécution

run: $(EXEC)
	./$(EXEC) ./Instances_MKP/100M5_1.txt 4

run2: $(EXEC)
	./$(EXEC) ./Instances_MKP/250M5_21.txt 4

# Compiler le benchmark (sans exécution)
bench: $(BENCH_EXEC)

# Exécuter le benchmark
benchr: $(BENCH_EXEC)
	./$(BENCH_EXEC)  ./Instances_MKP/500M30_21.txt 2
	
# Vous devez avoir doxygene d'installer
docs:
	doxygen Doxyfile

# windows
clean-docs:
	rd /s /q docs

# linux
clean-docsl:
	rm -rf docs

# Lancer le conteneur et accéder au shell
docker-shell:
	docker-compose up -d
	docker exec -it knapsack-container /bin/bash

# Générer la documentation avec docker
docker-docs:
	docker-compose up -d
	docker exec -it knapsack-container doxygen Doxyfile
