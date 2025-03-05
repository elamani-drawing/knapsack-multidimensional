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

    KnapsackInstance ksInstance;
    read_knapsack_file(argv[1], &ksInstance);
    // print_knapsack_instance(&ksInstance);

    // Initialiser une solution vide
    // KnapsackSolution *ksSolution = init_solution(ksInstance.n);

    // // (Exemple) Ajouter les objets 1, 3 et 5 à la solution
    // ksSolution->x[0] = 1;
    // ksSolution->x[2] = 1;
    // ksSolution->x[4] = 1;

    // // Calculer la valeur de la solution
    // evaluate_solution(ksSolution, &ksInstance);

    // // Vérifier si la solution est valide
    // if (is_feasible(ksSolution, &ksInstance)) {
    //     printf("Solution valide !\n");
    // } else {
    //     printf("Solution invalide !\n");
    // }

    // random_construction(ksSolution, &ksInstance);
    // greedy_construction(ksSolution, &ksInstance);
    KnapsackSolution *ksSolution = random_initial_solution(&ksInstance);
    // KnapsackSolution *ksSolution = greedy_initial_solution(&ksInstance);
    // Afficher et sauvegarder la solution
    print_solution(ksSolution, &ksInstance);
    print_solution_index(ksSolution, ksInstance.n);
    // save_solution("solution.txt", solution, ksInstance.n);

    // Libére la mémoire
    free_solution(ksSolution);
    free_knapsack_instance(&ksInstance);

    return EXIT_SUCCESS;
}