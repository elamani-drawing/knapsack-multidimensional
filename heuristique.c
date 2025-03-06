#include "heuristique.h"


KnapsackSolution *random_initial_solution(const KnapsackInstance *instance)
{
    KnapsackSolution *solution = init_solution(instance->n);

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

void local_search_1_flip(KnapsackSolution *solution, const KnapsackInstance *instance)
{
    int improved = 1;

    // Continue la recherche tant qu'il y a des améliorations possibles
    while (improved)
    {
        improved = 0;

        // Tester chaque objet en l'ajoutant ou en le retirant de la solution
        for (int i = 0; i < instance->n; i++)
        {
            // Sauvegarder l'ancienne valeur de la solution
            int old_Z = solution->Z;

            // Effectuer le flip (inversion de l'état de l'objet)
            solution->x[i] = 1 - solution->x[i];

            // Vérifier si la solution est toujours faisable
            if (is_feasible(solution, instance))
            {
                // Calculer la nouvelle valeur de la solution
                evaluate_solution(solution, instance);

                // Si la solution s'est améliorée, garder ce flip
                if (solution->Z > old_Z)
                {
                    improved = 1;
                    // break; // Sortir de la boucle for pour recommencer l'exploration
                }
                else
                {
                    // Annuler le flip si la solution ne s'est pas améliorée
                    solution->x[i] = 1 - solution->x[i];
                    solution->Z = old_Z;
                }
            }
            else
            {
                // Annuler le flip si la solution devient invalide
                solution->x[i] = 1 - solution->x[i];
            }
        }
    }
}

void local_search_swap(KnapsackSolution *solution, const KnapsackInstance *instance)
{
    int improved = 1;

    // Continue la recherche tant qu'il y a des améliorations possibles
    while (improved)
    {
        improved = 0;

        // Tester tous les échanges possibles
        for (int i = 0; i < instance->n; i++)
        {
            for (int j = i + 1; j < instance->n; j++)
            {
                // Si un objet est sélectionné et l'autre ne l'est pas, on les échange
                if (solution->x[i] != solution->x[j])
                {
                    // Sauvegarder l'ancienne valeur de la solution
                    int old_Z = solution->Z;

                    // Échanger les objets i et j
                    solution->x[i] = 1 - solution->x[i];
                    solution->x[j] = 1 - solution->x[j];

                    // Vérifier si la solution reste faisable après l'échange
                    if (is_feasible(solution, instance))
                    {
                        // Calculer la nouvelle valeur de la solution
                        evaluate_solution(solution, instance);

                        // Si la solution s'est améliorée, garder l'échange
                        if (solution->Z > old_Z)
                        {
                            improved = 1;
                            break; // Sortir de la boucle for pour recommencer l'exploration
                        }
                        else
                        {
                            // Annuler l'échange si la solution ne s'est pas améliorée
                            solution->x[i] = 1 - solution->x[i];
                            solution->x[j] = 1 - solution->x[j];
                            solution->Z = old_Z;
                        }
                    }
                    else
                    {
                        // Annuler l'échange si la solution devient invalide
                        solution->x[i] = 1 - solution->x[i];
                        solution->x[j] = 1 - solution->x[j];
                    }
                }
            }
            if (improved)
                break; // Sortir de la boucle for externe si une amélioration est trouvée
        }
    }
}


void variable_neighborhood_descent(KnapsackSolution *solution, const KnapsackInstance *instance, int time_limit)  {
    if (time_limit > 0) {
        timeout_flag = 0;
        start_time = get_current_time();
        if (setjmp(env) != 0) {
            printf("Temps écoulé ! Arrêt de l'algorithme (variable_neighborhood_descent).\n");
            return;  // Sortir de la fonction si le temps est écoulé
        }
    }
    int neighborhood = 1; // 1 = flip_1, 2 = swap
    int improved;

    do {
        improved = 0;
        if (neighborhood == 1) {
            int old_Z = solution->Z;
            local_search_1_flip(solution, instance);
            if (solution->Z > old_Z) {
                improved = 1;
                neighborhood = 1; // Revenir au premier voisinage
            } else {
                neighborhood = 2; // Passer à swap
            }
        } else if (neighborhood == 2) {
            int old_Z = solution->Z;
            local_search_swap(solution, instance);
            if (solution->Z > old_Z) {
                improved = 1;
                neighborhood = 1; // Revenir à flip_1
            }
        }
        // Vérification du timeout à chaque itération
        if (time_limit > 0) check_timeout(start_time, time_limit);
    } while (improved);
}


void random_flip(KnapsackSolution *solution, const KnapsackInstance *instance, int k_perturbation) {

    for (int p = 0; p < k_perturbation; p++) {
        // Choisir un objet aléatoire
        int i = rand() % instance->n;

        // Inverser l'état de l'objet
        solution->x[i] = 1 - solution->x[i];

        // Vérifier si la solution reste faisable
        if (!is_feasible(solution, instance)) {
            // Annuler la perturbation si la solution devient invalide
            solution->x[i] = 1 - solution->x[i];
        }
    }
}

void variable_neighborhood_search(KnapsackSolution *solution, const KnapsackInstance *instance, int max_iterations, int k_perturbation, int time_limit) {

    if (time_limit > 0) {
        timeout_flag = 0;
        start_time = get_current_time();
        if (setjmp(env) != 0) {
            printf("Temps écoulé ! Arrêt de l'algorithme (variable_neighborhood_search).\n");
            return;  // Sortir de la fonction si le temps est écoulé
        }
    }
    int iteration = 0;

    // Initialiser la meilleure solution avec init_solution
    KnapsackSolution *best_solution = init_solution(instance->n);

    while (iteration < max_iterations) {
        // Phase de VND
        variable_neighborhood_descent(solution, instance, 0);

        // Sauvegarder la meilleure solution trouvée (copie profonde)
        if (solution->Z > best_solution->Z) {
            copy_knapsack_solution(best_solution, solution, instance->n);
        }

        // Phase de perturbation
        random_flip(solution, instance, k_perturbation);

        // Phase de VND après perturbation
        variable_neighborhood_descent(solution, instance, 0);

        // Si la solution après perturbation est meilleure, la conserver
        if (solution->Z > best_solution->Z) {
            copy_knapsack_solution(best_solution, solution, instance->n);
        } else {
            // Sinon, revenir à la meilleure solution précédente
            copy_knapsack_solution(solution, best_solution, instance->n);
        }

        iteration++;
        // Vérification du timeout à chaque itération
        if (time_limit > 0) check_timeout(start_time, time_limit);
    }

    // Libérer la mémoire de la meilleure solution
    free_solution(best_solution);
}