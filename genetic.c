#include "genetic.h"

#include "genetic.h"

Individual *init_individual(int n)
{
    Individual *individual = (Individual *)malloc(sizeof(Individual));
    if (!individual) {
        // Gestion d'erreur en cas d'échec d'allocation mémoire
        perror("Erreur d'allocation mémoire pour individu");
        return NULL;
    }

    individual->solution = init_solution(n); // Initialisation de la solution
    if (!individual->solution) {
        // Gestion d'erreur en cas d'échec de l'initialisation de la solution
        free(individual);
        return NULL;
    }

    individual->fitness = 0;
    return individual;
}


void free_individual(Individual *individual) {
    if (individual != NULL) {
        if (individual->solution != NULL) {
            free_solution(individual->solution); // Libère la mémoire de KnapsackSolution
        }
        // Pas besoin de libérer individual lui-même car il est généralement alloué dans un tableau
    }
}


void free_population(Individual *population, int population_size) {
    if (population != NULL) {
        for (int i = 0; i < population_size; i++) {
            free_individual(&population[i]); // Libère chaque individu
        }
        free(population); // Libère le tableau lui-même
    }
}

// Fonction de sélection par tournoi
Individual* tournament_selection(Individual *population, int population_size) {
    int i1 = rand() % population_size;
    int i2 = rand() % population_size;
    return (population[i1].fitness > population[i2].fitness) ? &population[i1] : &population[i2];
}

// Fonction de croisement en un point
void crossover(KnapsackSolution *parent1, KnapsackSolution *parent2, KnapsackSolution *child, const KnapsackInstance *instance) {
    int point = rand() % instance->n;
    for (int i = 0; i < point; i++) {
        child->x[i] = parent1->x[i];
    }
    for (int i = point; i < instance->n; i++) {
        child->x[i] = parent2->x[i];
    }
    evaluate_solution(child, instance);
    if (!is_feasible(child, instance)) {
        copy_knapsack_solution(child, parent1, instance->n);
    }
}

// Fonction de mutation
void mutate(KnapsackSolution *solution, const KnapsackInstance *instance, double mutation_rate) {
    if ((double)rand() / RAND_MAX < mutation_rate) {
        KnapsackSolution *temp = init_solution(instance->n);
        copy_knapsack_solution(temp, solution, instance->n);
        random_flip(temp, instance, 1);
        if (is_feasible(temp, instance)) {
            copy_knapsack_solution(solution, temp, instance->n);
        }
        free_solution(temp);
    }
}

// Fonction pour exécuter l'algorithme génétique
KnapsackSolution* genetic_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate) {
    Individual *population = malloc(population_size * sizeof(Individual));
    if (!population)
    {
        perror("Erreur d'allocation mémoire pour population (genetic_algorithm)");
        return NULL;
    }
    
    // Initialisation de la population
    for (int i = 0; i < population_size; i++) {
        population[i].solution = random_initial_solution(instance);
        evaluate_solution(population[i].solution, instance);
        population[i].fitness = population[i].solution->Z;
    }

    for (int gen = 0; gen < generations; gen++) {
        Individual *new_population = malloc(population_size * sizeof(Individual));
        if (!new_population) {
            // Libère la mémoire de la population
            free_population(population, population_size); 
            return NULL;
        }
        
        for (int i = 0; i < population_size; i++) {
            // Sélection
            Individual *parent1 = tournament_selection(population, population_size);
            Individual *parent2 = tournament_selection(population, population_size);
            
            // Croisement
            new_population[i].solution = init_solution(instance->n);
            crossover(parent1->solution, parent2->solution, new_population[i].solution, instance);
            
            // Mutation
            mutate(new_population[i].solution, instance, mutation_rate);
            
            // Évaluation
            evaluate_solution(new_population[i].solution, instance);
            new_population[i].fitness = new_population[i].solution->Z;
        }
        
        // Remplacement de l'ancienne population
        for (int i = 0; i < population_size; i++) {
            free_individual(&population[i]); 
            population[i] = new_population[i];
        }
        free(new_population);
    }
    
    // Retourne la meilleure solution trouvée
    // Individual *best =  init_individual(instance->n);
    Individual *best = &population[0];
    for (int i = 0; i < population_size; i++) {
        if (population[i].fitness > best->fitness) {
            best = &population[i];
        }
    }
    // Vérifie que best->solution est initialisé
    if (best == NULL || best->solution == NULL)  {
        // Gestion d'erreur : best->solution n'est pas initialisé
        free_population(population, population_size);
        return NULL;
    }
    
    KnapsackSolution *best_solution = init_solution(instance->n);
    copy_knapsack_solution(best_solution, best->solution, instance->n);
    
    // Libère la mémoire de la population
    free_population(population, population_size); 
    
    return best_solution;
}

KnapsackSolution* hybrid_GA_VNS(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int vns_iterations, int k) {
    // Initialisation de la population
    Individual *population = malloc(population_size * sizeof(Individual));
    if (!population)
    {
        perror("Erreur d'allocation mémoire pour population (hybrid_GA_VNS)");
        return NULL;
    }
    
    for (int i = 0; i < population_size; i++) {
        population[i].solution = random_initial_solution(instance);
        population[i].fitness = population[i].solution->Z;
    }

    // Boucle principale sur les générations
    for (int gen = 0; gen < generations; gen++) {
        // 3. Sélection, croisement et mutation
        Individual *new_population = malloc(population_size * sizeof(Individual));
        for (int i = 0; i < population_size; i++) {
            // Sélection par tournoi
            Individual *parent1 = tournament_selection(population, population_size);
            Individual *parent2 = tournament_selection(population, population_size);

            // Croisement
            KnapsackSolution *child = init_solution(instance->n);
            crossover(parent1->solution, parent2->solution, child, instance);

            // Mutation
            mutate(child, instance, mutation_rate);

            // Évaluation de l'enfant
            evaluate_solution(child, instance);

            // Ajout à la nouvelle population
            new_population[i].solution = child;
            new_population[i].fitness = child->Z;
        }

        // Application du VNS sur chaque individu de la nouvelle population
        for (int i = 0; i < population_size; i++) {
            variable_neighborhood_search(new_population[i].solution, instance, vns_iterations, k);
            evaluate_solution(new_population[i].solution, instance); // Recalcul de la fitness après VNS
        }

        // Remplacement de l'ancienne population par la nouvelle
        free_population(population, population_size);
        population = new_population;
    }

    // Retour de la meilleure solution trouvée
    // Individual *best_individual =  init_individual(instance->n);
    Individual *best_individual = &population[0];
    for (int i = 0; i < population_size; i++) {
        if (population[i].fitness > best_individual->fitness) {
            best_individual = &population[i];
        }
    }

    // Vérifie que best_individual->solution est initialisé
    if (best_individual == NULL || best_individual->solution == NULL) {
        // Gestion d'erreur : best->solution n'est pas initialisé
        free_population(population, population_size);
        return NULL;
    }
    KnapsackSolution *best_solution = init_solution(instance->n);
    copy_knapsack_solution(best_solution, best_individual->solution, instance->n);
    
    free_population(population, population_size);
    return best_solution;
}