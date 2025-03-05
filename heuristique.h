#include "knapsack.h"
#include <time.h>

/**
 * @brief Construit une solution initiale aléatoire pour le problème du sac à dos multidimensionnel.
 *
 * Cette fonction sélectionne aléatoirement des objets pour créer une solution.
 * Si la solution générée dépasse les contraintes de capacité, des objets sont retirés aléatoirement
 * jusqu'à ce que la solution soit faisable.
 *
 * @param solution Pointeur vers la solution à initialiser.
 * @param instance Pointeur vers l'instance du problème du sac à dos multidimensionnel.
 */
void random_construction(KnapsackSolution *solution, const KnapsackInstance *instance);

/**
 * @brief Génère une solution initiale aléatoire pour le problème du sac à dos.
 *
 * Cette fonction génère une solution aléatoire où chaque objet est inclus
 * dans la solution si son inclusion ne viole aucune des contraintes de capacité.
 * La solution est générée en choisissant des objets aléatoires et en vérifiant
 * si leur ajout respecte les capacités des contraintes.
 *
 * @param instance L'instance du problème de sac à dos, contenant les objets, leurs poids
 *                 et les capacités des contraintes.
 * @return Un pointeur vers une structure `KnapsackSolution` représentant la solution
 *         générée. La solution est valide si l'ajout des objets respecte toutes
 *         les contraintes de capacité. Si la solution ne peut pas être générée
 *         en raison d'une erreur de mémoire, un comportement indéfini peut se produire.
 */
KnapsackSolution *random_initial_solution(const KnapsackInstance *instance);

/**
 * @brief Fonction de comparaison utilisée pour trier les objets du problème du sac à dos en fonction du ratio profit/poids,
 *        en tenant compte de toutes les contraintes.
 *
 * Cette fonction est utilisée par `qsort_s` pour trier les indices des objets du sac à dos dans l'ordre décroissant de leur ratio
 * profit/poids. Le ratio est calculé en prenant en compte tous les poids normalisés par les capacités des différentes contraintes.
 *
 * @param context Pointeur vers l'instance du problème du sac à dos (contenant les données de l'instance comme les poids,
 *                les profits et les capacités des contraintes).
 * @param a Pointeur vers l'indice du premier objet à comparer.
 * @param b Pointeur vers l'indice du deuxième objet à comparer.
 *
 * @return Un entier négatif si le ratio de l'objet a est plus grand que celui de l'objet b,
 *         un entier positif si le ratio de l'objet a est plus petit que celui de l'objet b,
 *         et zéro si les deux objets ont le même ratio.
 */
int compare_knapsack_instance(void *context, const void *a, const void *b);

/**
 * @brief Fonction pour générer une solution initiale pour le problème du sac à dos en utilisant une approche gloutonne.
 *
 * Cette fonction crée une solution initiale en sélectionnant les objets dans l'ordre décroissant de leur ratio
 * profit/poids. Les objets sont ajoutés à la solution tant qu'ils respectent les contraintes de capacité.
 * Si l'ajout d'un objet dépasse la capacité d'une des contraintes, cet objet est ignoré.
 *
 * @param instance Pointeur vers l'instance du problème du sac à dos (contenant les données comme les poids, les profits,
 *                 et les capacités des contraintes).
 *
 * @return Pointeur vers une structure `KnapsackSolution` contenant la solution initiale générée.
 *         La solution contient les objets sélectionnés (`x[i] = 1` pour un objet sélectionné, `x[i] = 0` sinon) et la
 *         valeur de la solution est calculée via `evaluate_solution`.
 *
 * @note Cette fonction utilise l'algorithme glouton, qui sélectionne les objets en fonction de leur rentabilité,
 *       sans chercher à optimiser globalement la solution.
 */
KnapsackSolution *greedy_initial_solution(const KnapsackInstance *instance);