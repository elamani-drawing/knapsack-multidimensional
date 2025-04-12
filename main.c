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

    int temps_max = atoi(argv[2]);

    // KnapsackSolution *ksSolution = genetic_algorithm(&ksInstance, 5000, 5000, 0.05, temps_max); // population, generations, mutation_rate, temps_max
    KnapsackSolution *ksSolution = hybrid_GA_VNS(&ksInstance, 100, 100, 0.05, 100, 2,temps_max, 1); // population, generations, mutation_rate, vns_iteration, k, temps_max, neighborhood (flip_1)


    // KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // KnapsackSolution *ksSolution = greedy_initial_solution(&ksInstance);


    // KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // printf("Avant flip : Z = %d\n", ksSolution->Z);
    // local_search_1_flip(ksSolution, &ksInstance);


    // KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // printf("Avant swap : Z = %d\n", ksSolution->Z);
    // local_search_swap(ksSolution, &ksInstance);


    // KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // printf("Avant neighborhood descent : Z = %d\n", ksSolution->Z);
    // variable_neighborhood_descent(ksSolution, &ksInstance, temps_max, 1); //  temps_max, 1 (flip_1)


    // // Appliquer la recherche à voisinage variable (VNS)
    // KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // printf("Avant VNS descent : Z = %d\n", ksSolution->Z);
    // variable_neighborhood_search(ksSolution, &ksInstance, 3555555, 4, temps_max, 1); // temps_max, 1 (flip_1)


    print_solution(ksSolution, &ksInstance);
    print_solution_index(ksSolution, ksInstance.n);

    save_solution_to_file(ksSolution, &ksInstance, "solution.txt");
    // Libére la mémoire
    free_solution(ksSolution);
    free_knapsack_instance(&ksInstance);

    return EXIT_SUCCESS;
}