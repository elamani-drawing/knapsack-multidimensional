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


/**
 * @brief Applique la recherche locale 1-flip pour améliorer la solution actuelle.
 *
 * Cette fonction effectue un flip (ajout ou suppression d'un objet) pour chaque objet et
 * met à jour la solution si cela améliore la faisabilité et le profit.
 *
 * @param solution La solution actuelle à améliorer.
 * @param instance L'instance du problème de sac à dos, contenant les poids et les profits des objets.
 */
void local_search_1_flip(KnapsackSolution *solution, const KnapsackInstance *instance);

/**
 * @brief Applique la recherche locale par échange pour améliorer la solution actuelle.
 *
 * Cette fonction effectue un échange entre deux objets sélectionnés et non sélectionnés
 * et met à jour la solution si cela améliore la faisabilité et le profit.
 *
 * @param solution La solution actuelle à améliorer.
 * @param instance L'instance du problème de sac à dos, contenant les poids et les profits des objets.
 */
void local_search_swap(KnapsackSolution *solution, const KnapsackInstance *instance);

/**
 * @brief Applique la méthode de descente de voisinage variable (VND) pour améliorer une solution du problème du sac à dos.
 *
 * La descente de voisinage variable (VND) utilise deux stratégies de recherche locale :
 * 1. `local_search_1_flip` : Modifie l'état individuel des objets (ajout/retrait) pour améliorer la solution.
 * 2. `local_search_swap` : Échange deux objets (un inclus, l'autre exclu) pour optimiser la solution.
 *
 * La fonction commence avec la recherche `flip_1`. Lorsqu'elle atteint un optimum local, elle passe à `swap`.
 * Si `swap` améliore la solution, elle retourne à `flip_1` pour exploiter les nouvelles opportunités.
 * Le processus continue jusqu'à ce qu'aucune amélioration ne soit possible avec les deux voisinages.
 *
 * @param solution Pointeur vers la solution actuelle du problème du sac à dos.
 * @param instance Pointeur vers l'instance du problème du sac à dos contenant les données (poids, valeurs, capacité, etc.).
 *
 * @note Cette méthode garantit une amélioration progressive de la solution, mais ne garantit pas d'atteindre l'optimum global.
 * @note La solution initiale doit être définie avant d'appeler cette fonction.
 */
void variable_neighborhood_descent(KnapsackSolution *solution, const KnapsackInstance *instance);
