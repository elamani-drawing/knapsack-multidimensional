#include "knapsack.h"

void read_knapsack_file(const char *filename, KnapsackInstance *instance)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Lire n et m
    if (fscanf(file, "%d %d", &instance->n, &instance->m) != 2)
    {
        perror("Erreur lors de la lecture de n et m");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Allocation mémoire
    instance->profits = (int *)malloc(instance->n * sizeof(int));
    if (!instance->profits)
    {
        perror("Erreur d'allocation mémoire pour les profits");
        exit(EXIT_FAILURE);
    }
    instance->capacities = (int *)malloc(instance->m * sizeof(int));
    if (!instance->capacities)
    {
        perror("Erreur d'allocation mémoire pour les capacités");
        free(instance->profits);
        exit(EXIT_FAILURE);
    }

    instance->weights = (int **)malloc(instance->m * sizeof(int *));
    if (!instance->weights)
    {
        perror("Erreur d'allocation mémoire pour les poids");
        free(instance->profits);
        free(instance->capacities);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < instance->m; i++)
    {
        instance->weights[i] = (int *)malloc(instance->n * sizeof(int));
        if (!instance->weights[i])
        {
            perror("Erreur d'allocation mémoire pour les poids");
            for (int j = 0; j < i; j++)
            {
                free(instance->weights[j]);
            }
            free(instance->weights);
            free(instance->profits);
            free(instance->capacities);
            exit(EXIT_FAILURE);
        }
    }

    // Lire les coefficients des objets (couts/valeur/profit)
    for (int i = 0; i < instance->n; i++)
    {
        if (fscanf(file, "%d", &instance->profits[i]) != 1) {
            // Gérer l'erreur de lecture
            fprintf(stderr, "Erreur de lecture des profits\n");
            exit(EXIT_FAILURE);
        }
    }

    // Lire les capacités des contraintes
    for (int i = 0; i < instance->m; i++)
    {
        if (fscanf(file, "%d", &instance->capacities[i]) != 1) {
            // Gérer l'erreur de lecture
            fprintf(stderr, "Erreur de lecture des capacités\n");
            exit(EXIT_FAILURE);
        }
    }

    // Lire les poids des objets pour chaque contrainte
    for (int i = 0; i < instance->m; i++)
    {
        for (int j = 0; j < instance->n; j++)
        {
            if (fscanf(file, "%d", &instance->weights[i][j]) != 1) {
                // Gérer l'erreur de lecture
                fprintf(stderr, "Erreur de lecture des poids\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

void free_knapsack_instance(KnapsackInstance *instance)
{
    free(instance->profits);
    free(instance->capacities);
    for (int i = 0; i < instance->m; i++)
    {
        free(instance->weights[i]);
    }
    free(instance->weights);
}

void print_knapsack_instance(const KnapsackInstance *instance)
{
    printf("\n-Nombre d'objets: %d, Nombre de contraintes: %d\n", instance->n, instance->m);
    printf("\n-Profits: ");
    for (int i = 0; i < instance->n; i++)
    {
        printf("%d ", instance->profits[i]);
    }
    printf("\n-Capacités: ");
    for (int i = 0; i < instance->m; i++)
    {
        printf("%d ", instance->capacities[i]);
    }
    printf("\n-Poids:\n");
    for (int i = 0; i < instance->m; i++)
    {
        printf("--Poids pour la contraintes %d:\n", i + 1);
        for (int j = 0; j < instance->n; j++)
        {
            printf("%d ", instance->weights[i][j]);
        }
        printf("\n");
    }
}

int *copy_capacities(const KnapsackInstance *instance) {
    int *remaining_capacities = (int *)malloc(instance->m * sizeof(int));
    if (!remaining_capacities) {
        return NULL;
    }
    for (int j = 0; j < instance->m; j++) {
        remaining_capacities[j] = instance->capacities[j];
    }
    return remaining_capacities;
}


////////////////////////////////////////////////////////////////// Solution

KnapsackSolution *init_solution(int n)
{
    KnapsackSolution *solution = (KnapsackSolution *)malloc(sizeof(KnapsackSolution));
    solution->x = (int *)calloc(n, sizeof(int)); // Initialise tous les objets à 0
    solution->Z = 0;                             // Valeur initiale de la solution à 0
    return solution;
}

void copy_knapsack_solution(KnapsackSolution *dest, const KnapsackSolution *src, int n) {
    dest->Z = src->Z; // Copier la valeur de la solution
    dest->x = (int *)malloc(n * sizeof(int)); // Allouer de la mémoire pour le tableau x
    if (dest->x == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée (copy_knapsack_solution).\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        dest->x[i] = src->x[i]; // Copier les éléments du tableau x
    }
}

void reset_solution(KnapsackSolution *solution, int n)
{
    // Réinitialisation de la solution à zéro (aucun objet sélectionné)
    for (int i = 0; i < n; i++)
    {
        solution->x[i] = 0;
    }

    // Réinitialisation de la valeur de la solution à 0
    solution->Z = 0;
}

void evaluate_solution(KnapsackSolution *solution, const KnapsackInstance *instance)
{
    solution->Z = 0;
    for (int i = 0; i < instance->n; i++)
    {
        if (solution->x[i] == 1)
        {
            solution->Z += instance->profits[i];
        }
    }
}

int is_feasible(const KnapsackSolution *solution, const KnapsackInstance *instance)
{
    for (int j = 0; j < instance->m; j++)
    {
        int total_weight = 0;
        for (int i = 0; i < instance->n; i++)
        {
            if (solution->x[i] == 1)
            {
                total_weight += instance->weights[j][i];
            }
        }
        if (total_weight > instance->capacities[j])
        {
            return 0; // La solution est invalide
        }
    }
    return 1; // La solution est valide
}


int solution_length(const KnapsackSolution *solution, const KnapsackInstance *instance) {
    int count = 0;
    
    // Parcourir tous les objets et compter ceux qui sont sélectionnés (x[i] == 1)
    for (int i = 0; i < instance->n; i++) {
        if (solution->x[i] == 1) {
            count++;
        }
    }
    
    return count;
}

void print_solution(const KnapsackSolution *solution, const KnapsackInstance *instance)
{
    printf("\nSolution sélectionnée (Valeurs des objets) :\n");
    int has_objects = 0;
    for (int i = 0; i < instance->n; i++)
    {
        if (solution->x[i] == 1)
        {  
            printf("%d ", instance->profits[i]);
            has_objects = 1;
        }
    }
    if (!has_objects)
    {
        printf("Aucun objet sélectionné.");
    }
    printf("\nValeur totale de la solution : %d\n", solution->Z);
}

void print_solution_index(const KnapsackSolution *solution, int n)
{
    printf("\nIndices des objets sélectionnés :\n");
    int has_objects = 0;
    for (int i = 0; i < n; i++)
    {
        if (solution->x[i] == 1)
        {
            printf("%d ", i);
            has_objects = 1;
        }
    }
    if (!has_objects)
    {
        printf("Aucun objet sélectionné.");
    }
    printf("\n");
}

void free_solution(KnapsackSolution *solution)
{
    free(solution->x);
    free(solution);
}

void save_solution_to_file(const KnapsackSolution *solution, const KnapsackInstance *instance, const char *filename) {
    // Ouvrir le fichier en mode écriture
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // 1ère ligne : Valeur de la solution et nombre d'objets sélectionnés
    int selected_count = 0;
    for (int i = 0; i < instance->n; i++) {
        if (solution->x[i] == 1) {
            selected_count++;
        }
    }
    fprintf(file, "%d %d\n", solution->Z, selected_count);

    // 2ème ligne : Liste des indices des objets sélectionnés
    for (int i = 0; i < instance->n; i++) {
        if (solution->x[i] == 1) {
            fprintf(file, "%d ", i);
        }
    }
    fprintf(file, "\n");

    // Fermer le fichier
    fclose(file);
}