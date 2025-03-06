# Projet : knapsack-multidimensional

## Introduction

Ce projet vise à résoudre le problème du sac à dos multidimensionnel (Multidimensional Knapsack Problem, MKP), une généralisation du problème classique du sac à dos. Dans ce problème, chaque objet a un poids dans plusieurs dimensions (contraintes), et l'objectif est de maximiser la valeur totale des objets sélectionnés sans dépasser les capacités dans chaque dimension. Ce problème est NP-difficile, ce qui signifie qu'il n'existe pas d'algorithme efficace pour le résoudre exactement dans tous les cas. Par conséquent, nous utilisons des méthodes heuristiques et métaheuristiques pour trouver des solutions de bonne qualité en un temps raisonnable.

## Méthodes implémentées

Dans ce projet, nous avons implémenté plusieurs méthodes pour résoudre le MKP, conformément aux exigences du sujet, ainsi que des méthodes et structures supplémentaires (qui ne seront pas lister ici) pour améliorer la flexibilité, la compréhension du code et les performances. Voici une liste des fonctionnalités implémentées :

### Méthodes principales
1. **Algorithmes de génération de solutions initiales** :
   - `random_initial_solution` : Génère une solution initiale aléatoire.
   - `greedy_initial_solution` : Génère une solution initiale en utilisant une approche gloutonne.

2. **Algorithmes de recherche locale** :
   - `local_search_1_flip` : Améliore une solution en testant des modifications locales (flip).
   - `local_search_swap` : Améliore une solution en échangeant des objets.

3. **Algorithmes de recherche à voisinage variable (VNS)** :
   - `variable_neighborhood_descent` : Applique une descente dans plusieurs voisinages pour améliorer une solution.
   - `variable_neighborhood_search` : Combine des phases de perturbation et de descente pour explorer l'espace des solutions.

4. **Algorithmes génétiques** (BONUS) :
   - `genetic_algorithm` : Implémente un algorithme génétique pour explorer l'espace des solutions.
   - `hybrid_GA_VNS` : Combine un algorithme génétique avec une recherche à voisinage variable pour améliorer les performances.

5. **Évaluation et validation** :
   - `evaluate_solution` : Calcule la valeur et la faisabilité d'une solution.
   - `is_feasible` : Vérifie si une solution respecte les contraintes du problème.

6. **Gestion des fichiers** :
   - `read_knapsack_file` : Lit une instance du problème à partir d'un fichier.
   - `save_solution_to_file` : Sauvegarde la solution trouvée dans un fichier.

### Structures de données
- `KnapsackInstance` : Représente une instance du problème (objets, contraintes, capacités).
- `KnapsackSolution` : Représente une solution (objets sélectionnés, valeur totale).

### Fonctionnalités supplémentaires
- Gestion du temps d'exécution (`time_limit`) pour limiter la durée des algorithmes.
- Utilisation de `setjmp` et `longjmp` pour interrompre les algorithmes si le temps est écoulé.
- Benchmarking pour comparer les performances des différentes méthodes.

## Comment lancer le projet

### Compilation
Le projet est compilé à l'aide d'un fichier `Makefile`. Voici les commandes disponibles :

1. **Compiler le projet** :
    - Pour construire l'executable principal :
    ```bash
    make
    ```
    - Pour lancer l'executable principal :
    ```bash
    ./sadm_solver.exe <fichier_instance> <temps_max>
    ```
2. **Compiler le benchmark** :
    - Pour construire l'executable pour les résultats expérimentaux :
    ```bash
    make
    ```
    - Pour lancer l'executable pour les résultats expérimentaux :
    ```bash
    ./sadm_bench.exe <fichier_instance> <temps_max>
    ```

3. **Compiler et exécuter le projet avec une instance spécifique** :
   - Pour lancer l'algorithme sur l'instance `100M5_1.txt` :
    ```bash
    make run
    ```
   - Pour lancer l'algorithme sur l'instance `250M5_21.txt` :
    ```bash
    make run2
    ```

4. **Compiler et exécuter le benchmark** :
   - Exécuter le benchmark sur l'instance `500M30_21.txt` :
    ```bash
    make benchr
    ```

### Exemples d'utilisation
Voici quelques exemples de code pour utiliser les différentes méthodes implémentées :

1. **Utiliser l'algorithme génétique** :
   ```c
   KnapsackInstance ksInstance;
   read_knapsack_file("Instances_MKP/100M5_1.txt", &ksInstance);
   KnapsackSolution *ksSolution = genetic_algorithm(&ksInstance, 5000, 5000, 0.05, 3); // 3 secondes de limite
   save_solution_to_file(ksSolution, &ksInstance, "solution.txt");
   ```

2. **Utiliser la recherche à voisinage variable (VNS)** :
   ```c
   KnapsackInstance ksInstance;
   read_knapsack_file("Instances_MKP/100M5_1.txt", &ksInstance);
   KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
   variable_neighborhood_search(ksSolution, &ksInstance, 1000, 4, 10); // 10 secondes de limite
   save_solution_to_file(ksSolution, &ksInstance, "solution.txt");
   ```

3. **Utiliser une solution gloutonne** :
   ```c
   KnapsackInstance ksInstance;
   read_knapsack_file("Instances_MKP/100M5_1.txt", &ksInstance);
   KnapsackSolution *ksSolution = greedy_initial_solution(&ksInstance);
   save_solution_to_file(ksSolution, &ksInstance, "solution.txt");
   ```

4. **Tester la faisabilité d'une solution** :
   ```c
   KnapsackInstance ksInstance;
   read_knapsack_file("Instances_MKP/100M5_1.txt", &ksInstance);
   KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
   if (is_feasible(ksSolution, &ksInstance)) {
       printf("La solution est faisable.\n");
   } else {
       printf("La solution n'est pas faisable.\n");
   }
   ```

## Conclusion

Ce projet propose une implémentation complète et flexible pour résoudre le problème du sac à dos multidimensionnel. En plus des méthodes demandées, nous avons ajouté des fonctionnalités supplémentaires pour améliorer la robustesse et les performances. 