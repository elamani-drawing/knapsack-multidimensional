#include <stdio.h>
#include <stdlib.h>
#include "heuristique.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <fichier_instance> <temps_max>\n", argv[0]);
        return 1;
    }
    srand(time(NULL));

    KnapsackInstance ksInstance;
    read_knapsack_file(argv[1], &ksInstance);
    
    KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // KnapsackSolution *ksSolution = greedy_initial_solution(&ksInstance);

    // printf("Avant flip : Z = %d\n", ksSolution->Z);
    // local_search_1_flip(ksSolution, &ksInstance);
    // local_search_swap(ksSolution, &ksInstance);

    // printf("Avant neighborhood descent : Z = %d\n", ksSolution->Z);
    // variable_neighborhood_descent(ksSolution, &ksInstance);

    // Appliquer la recherche à voisinage variable (VNS)
    printf("Avant VNS descent : Z = %d\n", ksSolution->Z);
    int max_iterations = 100; // Nombre maximum d'itérations
    int k = 2; // Nombre d'objets à perturber
    variable_neighborhood_search(ksSolution, &ksInstance, max_iterations, k);

    print_solution(ksSolution, &ksInstance);
    print_solution_index(ksSolution, ksInstance.n);

    // Libére la mémoire
    free_solution(ksSolution);
    free_knapsack_instance(&ksInstance);

    return EXIT_SUCCESS;
}