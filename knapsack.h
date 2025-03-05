#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Structure représentant une instance du problème du sac à dos multidimensionnel.
 */
typedef struct
{
    int n;           /**< Nombre d'objets */
    int m;           /**< Nombre de contraintes */
    int *profits;    /**< Coefficients des objets (valeurs/profits) */
    int *capacities; /**< Capacités des contraintes */
    int **weights;   /**< Matrice des poids des objets pour chaque contrainte */
} KnapsackInstance;

/**
 * @brief Lit une instance du problème du sac à dos à partir d'un fichier.
 *
 * @param filename Nom du fichier contenant l'instance.
 * @param instance Pointeur vers la structure où stocker les données lues.
 */
void read_knapsack_file(const char *filename, KnapsackInstance *instance);

/**
 * @brief Libère la mémoire allouée dynamiquement pour une instance du problème du sac à dos.
 *
 * @param instance Pointeur vers la structure dont la mémoire doit être libérée.
 */
void free_knapsack_instance(KnapsackInstance *instance);

/**
 * @brief Affiche le contenu d'une instance du problème du sac à dos.
 *
 * @param instance Pointeur vers la structure à afficher.
 */
void print_knapsack_instance(const KnapsackInstance *instance);

/**
 * @brief Copie les capacités des contraintes d'une instance de sac à dos.
 *
 * Cette fonction crée un tableau de tailles `m` (le nombre de contraintes) et
 * y copie les valeurs des capacités de l'instance de sac à dos. Elle permet
 * de préserver l'intégrité de l'instance originale en ne modifiant pas les
 * capacités directement dans celle-ci.
 *
 * @param instance L'instance du problème de sac à dos contenant les capacités.
 * @return Un pointeur vers un tableau d'entiers contenant les capacités copiées.
 *         Si une erreur d'allocation mémoire survient, la fonction retourne NULL.
 */
int *copy_capacities(const KnapsackInstance *instance);

////////////////////////////////////////////////////////////////// Solution

/**
 * Structure représentant une solution du problème du sac à dos multidimensionnel.
 */
typedef struct
{
    int *x; // Tableau binaire de sélection des objets (taille n avec n le nombre d'objet dans la KnapsackInstance), Si 1 object selectionner, sinon 0
    int Z;  // Valeur de la fonction objectif, initialisé à 0
} KnapsackSolution;

/**
 * @brief Initialise une solution vide pour le problème du sac à dos.
 *
 * Cette fonction alloue dynamiquement une structure KnapsackSolution
 * et initialise son tableau `x` à zéro, indiquant qu'aucun objet n'est sélectionné.
 *
 * @param n Nombre d'objets dans le problème du sac à dos.
 * @return Un pointeur vers la solution initialisée.
 */
KnapsackSolution *init_solution(int n);

/**
 * @brief Réinitialise la solution en mettant tous les objets à zéro et en réinitialisant la valeur de la solution.
 *
 * Cette fonction est utilisée pour réinitialiser une solution avant de commencer une nouvelle construction ou de modifier
 * la solution actuelle. Elle met à zéro tous les objets dans `solution->x`, signifiant qu'aucun objet n'est sélectionné,
 * et réinitialise la valeur `Z` à -1, ce qui indique que la valeur de la solution doit être recalculée.
 *
 * @param solution Pointeur vers la structure `KnapsackSolution` représentant la solution à réinitialiser.
 * @param n Le nombre d'objets dans l'instance du problème (taille de `solution->x`).
 *
 * @note Cette fonction est généralement utilisée au début de la construction d'une nouvelle solution ou avant de réutiliser
 *       une solution existante pour éviter les résidus d'une précédente solution.
 */
void reset_solution(KnapsackSolution *solution, int n);

/**
 * @brief Évalue la solution en calculant la valeur totale de la fonction objectif.
 *
 * Cette fonction additionne les profits des objets sélectionnés (où `x[i] == 1`)
 * et met à jour `solution->Z` avec la somme totale.
 *
 * @param solution Pointeur vers la solution à évaluer.
 * @param instance Pointeur vers l'instance du problème contenant les profits des objets.
 */
void evaluate_solution(KnapsackSolution *solution, const KnapsackInstance *instance);

/**
 * @brief Vérifie si une solution est faisable en respectant les contraintes du sac à dos.
 *
 * Cette fonction somme les poids des objets sélectionnés pour chaque contrainte
 * et vérifie qu'ils ne dépassent pas les capacités du sac à dos.
 *
 * @param solution Pointeur vers la solution à tester.
 * @param instance Pointeur vers l'instance du problème contenant les poids et les capacités.
 * @return 1 si la solution est faisable, 0 sinon.
 */
int is_feasible(const KnapsackSolution *solution, const KnapsackInstance *instance);

/**
 * @brief Affiche les valeurs (profits) des objets sélectionnés dans la solution.
 *
 * Cette fonction parcourt la solution et affiche les profits des objets sélectionnés.
 * Si aucun objet n'est sélectionné, un message approprié est affiché.
 *
 * @param solution Pointeur vers la structure représentant la solution du problème.
 * @param instance Pointeur vers la structure représentant l'instance du problème du sac à dos.
 */
void print_solution(const KnapsackSolution *solution, const KnapsackInstance *instance);

/**
 * @brief Affiche les indices des objets sélectionnés dans la solution.
 *
 * Cette fonction parcourt la solution et affiche les indices des objets sélectionnés.
 * Si aucun objet n'est sélectionné, un message approprié est affiché.
 *
 * @param solution Pointeur vers la structure représentant la solution du problème.
 * @param n Nombre total d'objets dans l'instance du problème.
 */
void print_solution_index(const KnapsackSolution *solution, int n);

/**
 * @brief Libère la mémoire allouée pour une solution du sac à dos.
 *
 * Cette fonction libère l'espace mémoire utilisé par le tableau `x`
 * ainsi que la structure `KnapsackSolution` elle-même.
 *
 * @param solution Pointeur vers la solution à libérer.
 */
void free_solution(KnapsackSolution *solution);

#endif // KNAPSACK_H
