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


/**
 * @struct ExperimentalResultsKSM
 * @brief Structure représentant les résultats d'expérimentations.
 *
 * @var ExperimentalResultsKSM::initial_value
 * Valeur de la solution initiale.
 *
 * @var ExperimentalResultsKSM::initial_time
 * Temps CPU pour obtenir la solution initiale.
 *
 * @var ExperimentalResultsKSM::ls_value
 * Valeur de la solution après recherche locale.
 *
 * @var ExperimentalResultsKSM::ls_time
 * Temps CPU pour la recherche locale.
 *
 * @var ExperimentalResultsKSM::vnd_value
 * Valeur de la solution après VND.
 *
 * @var ExperimentalResultsKSM::vnd_time
 * Temps CPU pour VND.
 *
 * @var ExperimentalResultsKSM::vns_value
 * Valeur de la solution après VNS.
 *
 * @var ExperimentalResultsKSM::vns_time
 * Temps CPU pour VNS.
 */
typedef struct {
    double initial_value; /**< Valeur de la solution initiale */
    double initial_time;  /**< Temps CPU pour la solution initiale */
    double ls_value;      /**< Valeur après recherche locale */
    double ls_time;       /**< Temps CPU pour recherche locale */
    double vnd_value;     /**< Valeur après VND */
    double vnd_time;      /**< Temps CPU pour VND */
    double vns_value;     /**< Valeur après VNS */
    double vns_time;      /**< Temps CPU pour VNS */
} ExperimentalResultsKSM;


/**
 * @brief Mesure le temps CPU écoulé depuis le démarrage du programme.
 *
 * @return Le temps CPU écoulé en secondes (type double).
 */
double get_cpu_time();

/**
 * @brief Exécute une série d'expériences pour évaluer les performances des différentes méthodes
 *        sur une instance de sac à dos donnée.
 *
 * @param instance L'instance du problème du sac à dos à résoudre.
 * @param initialization_function La fonction d'initialisation pour générer la solution initiale.
 *        Elle doit prendre un paramètre `KnapsackInstance *` et retourner un pointeur vers une solution de type `KnapsackSolution`.
 *
 * @return Les résultats expérimentaux, comprenant les valeurs de la solution et le temps CPU pour chaque étape.
 */
ExperimentalResultsKSM run_experiments(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *));



/**
 * @brief Affiche les résultats des expérimentations sous forme de tableau.
 *
 * @param results Les résultats expérimentaux à afficher (de type `ExperimentalResultsKSM`).
 */
void print_results_table(const ExperimentalResultsKSM *results);


#endif // BENCHMARK_H
