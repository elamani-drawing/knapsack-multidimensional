/**
 * @brief Fonction de comparaison pour trier les objets par ratio profit/poids dans l'ordre décroissant.
 *
 * Elle est passée en argument à la fonction `qsort` pour trier un tableau d'indices
 *
 * @param a Pointeur vers le premier élément à comparer.
 * @param b Pointeur vers le deuxième élément à comparer.
 * @return Un entier négatif si le premier ratio est supérieur au second, un entier positif si le premier ratio est
 *         inférieur au second, et 0 si les deux ratios sont égaux.
 *
 * @note La fonction `qsort` utilise cette fonction de comparaison pour trier les indices des objets.
 */
int compare_ratios(const void *a, const void *b) {
    double ratio_a = *((double *)a);
    double ratio_b = *((double *)b);
    if (ratio_a > ratio_b) return -1;
    if (ratio_a < ratio_b) return 1;
    return 0;
}