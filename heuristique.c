#include "heuristique.h"

#include <stdlib.h>
#include <time.h>

KnapsackSolution *random_initial_solution(const KnapsackInstance *instance)
{
    KnapsackSolution *solution = init_solution(instance->n);
    srand(time(NULL));

    // Copier les capacités pour ne pas modifier l'instance originale
    int *remaining_capacities = copy_capacities(instance);
    ;
    if (remaining_capacities == NULL)
    {
        perror("Erreur d'allocation mémoire pour remaining_capacities (greedy_initial_solution)");
        exit(EXIT_FAILURE);
    }

    // Générer une solution aléatoire
    for (int i = 0; i < instance->n; i++)
    {
        int object_index = rand() % instance->n; // Choisir un objet aléatoire
        solution->x[object_index] = 1;           // Essayer d'ajouter l'objet à la solution

        // Vérifier la faisabilité de la solution
        if (!is_feasible(solution, instance))
        {
            solution->x[object_index] = 0; // Retirer l'objet si la solution n'est pas valide
        }
    }

    // Évaluer la solution
    evaluate_solution(solution, instance);

    free(remaining_capacities);
    return solution;
}

void random_construction(KnapsackSolution *solution, const KnapsackInstance *instance)
{
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    for (int i = 0; i < instance->n; i++)
    {
        solution->x[i] = rand() % 2; // 0 ou 1, objet sélectionné ou non
    }

    // Si la solution dépasse les capacités, retirer des objets aléatoirement
    while (!is_feasible(solution, instance))
    {
        int rand_index = rand() % instance->n;
        /**
         * Nous pourrions faire un tri sur les objets selectionner puis les retirer
         * mais pour respecter la construction aleatoire nous retirerons les objets de
         * maniere aleatoire sans interferet
         */
        // Si on a un objet selectionner on le retire
        if (solution->x[rand_index] == 1)
        {
            solution->x[rand_index] = 0;
        }
    }

    // Mise à jour de la valeur de la solution
    evaluate_solution(solution, instance);
}

int compare_knapsack_instance(void *context, const void *a, const void *b)
{
    const KnapsackInstance *instance = (const KnapsackInstance *)context;
    int index_a = *(int *)a;
    int index_b = *(int *)b;

    // Calcul du ratio profit/poids pour chaque objet en prenant en compte toutes les contraintes
    double total_weight_a = 0.0;
    double total_weight_b = 0.0;

    // Somme des poids normalisés par les capacités pour chaque contrainte
    for (int j = 0; j < instance->m; j++)
    {
        if (instance->capacities[j] > 0) // Éviter la division par zéro
        {
            total_weight_a += instance->weights[j][index_a] / (double)instance->capacities[j];
            total_weight_b += instance->weights[j][index_b] / (double)instance->capacities[j];
        }
    }

    // Calcul du ratio profit/poids
    double ratio_a = (total_weight_a > 0) ? ((double)instance->profits[index_a] / total_weight_a) : 0;
    double ratio_b = (total_weight_b > 0) ? ((double)instance->profits[index_b] / total_weight_b) : 0;

    // Comparaison des ratios
    if (ratio_a > ratio_b)
        return -1; // Ordre décroissant
    if (ratio_a < ratio_b)
        return 1;
    return 0; // Si les ratios sont égaux
}

KnapsackSolution *greedy_initial_solution(const KnapsackInstance *instance)
{
    KnapsackSolution *solution = init_solution(instance->n);

    // Crée un tableau d'indices pour trier les objets
    int *indices = (int *)malloc(instance->n * sizeof(int));
    if (!indices)
    {
        perror("Erreur d'allocation mémoire pour indices (greedy_initial_solution)");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < instance->n; i++)
    {
        indices[i] = i;
    }

    // Trie les objets par ratio profit/poids décroissant
    qsort_s(indices, instance->n, sizeof(int), compare_knapsack_instance, (void *)instance);

    // Copier les capacités pour ne pas modifier l'instance originale
    int *remaining_capacities = copy_capacities(instance);
    ;
    if (remaining_capacities == NULL)
    {
        perror("Erreur d'allocation mémoire pour remaining_capacities (greedy_initial_solution)");
        exit(EXIT_FAILURE);
    }

    /*
    // Ajouter les objets un par un tant que les contraintes ne sont pas violées
    for (int i = 0; i < instance->n; i++)
    {
        int object_index = indices[i];

        // Vérifier si l'ajout de l'objet respecte toutes les contraintes
        int feasible = 1;
        for (int j = 0; j < instance->m; j++)
        {
            if (solution->x[object_index] == 1 ||
                instance->weights[j][object_index] > remaining_capacities[j])
            {
                feasible = 0;
                break;
            }
        }

        if (feasible)
        {
            solution->x[object_index] = 1; // Ajouter l'objet à la solution
            for (int j = 0; j < instance->m; j++)
            {
                remaining_capacities[j] -= instance->weights[j][object_index];
            }
        }
    }
    */

    // Ajouter les objets un par un tant que les contraintes ne sont pas violées
    for (int i = 0; i < instance->n; i++)
    {
        int object_index = indices[i];
        solution->x[object_index] = 1; // Essayer d'ajouter l'objet à la solution

        // Vérifier la faisabilité de la solution
        if (!is_feasible(solution, instance))
        {
            solution->x[object_index] = 0; // Retirer l'objet si la solution n'est pas valide
        }
    }

    // Évaluer la solution
    evaluate_solution(solution, instance);

    // Libérer la mémoire allouée
    free(indices);
    free(remaining_capacities);

    return solution;
}
