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

KnapsackSolution* genetic_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int time_limit) {
    // LARGE_INTEGER start_time;

    if (time_limit > 0) {
        timeout_flag = 0;
        start_time = get_current_time();
        if (setjmp(env) != 0) {
            printf("Temps écoulé ! Arrêt de l'algorithme (genetic_algorithm).");
            goto cleanup;
        }
    }

    static Individual *population = NULL;
    population = malloc(population_size * sizeof(Individual));
    if (!population) {
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
            free_population(population, population_size);
            return NULL;
        }
        
        for (int i = 0; i < population_size; i++) {
            Individual *parent1 = tournament_selection(population, population_size);
            Individual *parent2 = tournament_selection(population, population_size);
            
            new_population[i].solution = init_solution(instance->n);
            crossover(parent1->solution, parent2->solution, new_population[i].solution, instance);
            mutate(new_population[i].solution, instance, mutation_rate);
            evaluate_solution(new_population[i].solution, instance);
            new_population[i].fitness = new_population[i].solution->Z;
            
            if (time_limit > 0) check_timeout(start_time, time_limit);
        }
        
        for (int i = 0; i < population_size; i++) {
            free_individual(&population[i]);
            population[i] = new_population[i];
        }
        free(new_population);
    }

cleanup:
    if (population) {
        Individual *best = &population[0];
        for (int i = 1; i < population_size; i++) {
            if (population[i].fitness > best->fitness) {
                best = &population[i];
            }
        }
        if (best == NULL || best->solution == NULL) {
            free_population(population, population_size);
            return NULL;
        }
        
        KnapsackSolution *best_solution = init_solution(instance->n);
        copy_knapsack_solution(best_solution, best->solution, instance->n);
        
        free_population(population, population_size);
        return best_solution;
    }
    
    return NULL;
}



KnapsackSolution* hybrid_GA_VNS(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int vns_iterations, int k, int time_limit) {
    // LARGE_INTEGER start_time;

    if (time_limit > 0) {
        timeout_flag = 0;
        start_time = get_current_time();
        if (setjmp(env) != 0) {
            printf("Temps écoulé ! Arrêt de l'algorithme (hybrid_GA_VNS).");
            goto cleanup;
        }
    }

    static Individual *population = NULL; 
    population = malloc(population_size * sizeof(Individual));
    if (!population) {
        perror("Erreur d'allocation mémoire pour population (hybrid_GA_VNS)");
        return NULL;
    }

    for (int i = 0; i < population_size; i++) {
        population[i].solution = random_initial_solution(instance);
        population[i].fitness = population[i].solution->Z;
    }

    for (int gen = 0; gen < generations; gen++) {

        Individual *new_population = malloc(population_size * sizeof(Individual));
        if (!new_population) {
            perror("Erreur d'allocation mémoire pour new_population");
            break;
        }

        for (int i = 0; i < population_size; i++) {
            Individual *parent1 = tournament_selection(population, population_size);
            Individual *parent2 = tournament_selection(population, population_size);

            KnapsackSolution *child = init_solution(instance->n);
            crossover(parent1->solution, parent2->solution, child, instance);
            mutate(child, instance, mutation_rate);
            evaluate_solution(child, instance);

            new_population[i].solution = child;
            new_population[i].fitness = child->Z;

            // Vérification du timeout à chaque itération
            if (time_limit > 0) check_timeout(start_time, time_limit);
        }

        for (int i = 0; i < population_size; i++) {
            variable_neighborhood_search(new_population[i].solution, instance, vns_iterations, k, 0);
            evaluate_solution(new_population[i].solution, instance);

            // Vérification du timeout à chaque itération
            if (time_limit > 0) check_timeout(start_time, time_limit);
        }

        free_population(population, population_size);
        population = new_population;
    }

cleanup:
    // Vérifie si `population` est encore valide
    if (population) {  
        Individual *best_individual = &population[0];
        for (int i = 1; i < population_size; i++) {
            if (population[i].fitness > best_individual->fitness) {
                best_individual = &population[i];
            }
        }

        KnapsackSolution *best_solution = init_solution(instance->n);
        copy_knapsack_solution(best_solution, best_individual->solution, instance->n);

        free_population(population, population_size);
        return best_solution;
    }

    return NULL;  // Si `longjmp` a provoqué une erreur avant l'initiation de population
}
