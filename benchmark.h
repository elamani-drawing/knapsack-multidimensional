#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "heuristique.h"


typedef struct {
    double value;
    double time;
    int length;
} ResultEntry;

typedef struct {
    ResultEntry greedy_vns;
    ResultEntry random_vns;
} ExperimentalResultsKSM;


/**
 * @brief Mesure le temps CPU écoulé depuis le démarrage du programme.
 *
 * @return Le temps CPU écoulé en secondes (type double).
 */
double get_cpu_time();

/**
 * @brief  Exécute une expérience sur une instance de sac à dos donnée.
 *
 * @param instance L'instance du problème du sac à dos à résoudre.
 * @param initialization_function La fonction d'initialisation pour générer la solution initiale.
 *        Elle doit prendre un paramètre `KnapsackInstance *` et retourner un pointeur vers une solution de type `KnapsackSolution`.
 * @param temps_max Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 *
 * @return Les résultats expérimentaux, comprenant les valeurs de la solution et le temps CPU pour chaque étape.
 */
ResultEntry run_experiment(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *), int temps_max);


/**
 * @brief Exécute toutes les expériences sur une instance de sac à dos donnée.
 *
 * @param instance L'instance du problème du sac à dos à résoudre.
 * @param temps_max Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 *
 * @return Les résultats expérimentaux, comprenant les valeurs de la solution et le temps CPU pour chaque étape.
 */
ExperimentalResultsKSM run_all_experiments(const KnapsackInstance *instance, int temps_max);

/**
 * @brief Affiche les résultats des expérimentations sous forme de tableau.
 *
 * @param results Les résultats expérimentaux à afficher (de type `ExperimentalResultsKSM`).
 */
void print_results_table(const ExperimentalResultsKSM *results);


#endif // BENCHMARK_H
