#include "genetic.h"

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
    KnapsackSolution *ksSolution = genetic_algorithm(&ksInstance, 500, 500, 0.05, 3);
    KnapsackSolution *ksSolution = hybrid_GA_VNS(&ksInstance, 100, 100, 0.05, 100, 2,3);
    // KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // KnapsackSolution *ksSolution = greedy_initial_solution(&ksInstance);

    // printf("Avant flip : Z = %d\n", ksSolution->Z);
    // local_search_1_flip(ksSolution, &ksInstance);
    // local_search_swap(ksSolution, &ksInstance);

    // printf("Avant neighborhood descent : Z = %d\n", ksSolution->Z);
    // variable_neighborhood_descent(ksSolution, &ksInstance);

    // // Appliquer la recherche à voisinage variable (VNS)
    // printf("Avant VNS descent : Z = %d\n", ksSolution->Z);
    // int max_iterations = 100; // Nombre maximum d'itérations
    // int k = 2; // Nombre d'objets à perturber
    // variable_neighborhood_search(ksSolution, &ksInstance, max_iterations, k);

    print_solution(ksSolution, &ksInstance);
    print_solution_index(ksSolution, ksInstance.n);

    save_solution_to_file(ksSolution, &ksInstance, "solution.txt");
    // Libére la mémoire
    free_solution(ksSolution);
    free_knapsack_instance(&ksInstance);

    return EXIT_SUCCESS;
}