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
#include "genetic.h"

#define SIZE_RESULT_BUFFER 1024 

typedef struct {
    char filename[256]; // vns
    double value; // vns
    double time; // vns 
    int length; // vns
    char* type ; // vns_aleatoire, vns_gloutonne, genetic, hybrid
    int pop_size; // genetic
    double mutation_rate; // genetic
    int generations; // genetic
    int vns_iterations; // hybrid
    int k; // hybrid
} ResultEntry;

typedef struct {
    ResultEntry greedy_vns;
    ResultEntry random_vns;
    ResultEntry genetic;
    ResultEntry hybrid;
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
 * @param filename Le nom du fichier pour enregistrer les résultats.
 *
 * @return Les résultats expérimentaux, comprenant les valeurs de la solution et le temps CPU pour chaque étape.
 */
ResultEntry run_experiment(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *), int temps_max, const char *filename);


/**
 * @brief Exécute l'algorithme génétique sur une instance de sac à dos donnée.
 * 
 * @param instance L'instance du problème du sac à dos à résoudre.
 * @param population_size La taille de la population pour l'algorithme génétique.
 * @param generations Le nombre de générations à exécuter.
 * @param mutation_rate Le taux de mutation pour l'algorithme génétique.
 * @param time_limit Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 * @param filename Le nom du fichier pour enregistrer les résultats.
 * 
 * @return Les résultats de l'algorithme, comprenant la valeur de la solution, le temps CPU et la taille de la solution.
 * 
 * @note Cette fonction exécute l'algorithme génétique sur l'instance donnée et retourne les résultats sous forme de structure `ResultEntry`.
 *       Elle mesure le temps CPU écoulé pendant l'exécution et calcule la taille de la solution.
 */
ResultEntry run_hybrid_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int vns_iterations, int k, int time_limit, const char *filename);


/**
 * @brief Exécute l'algorithme génétique sur une instance de sac à dos donnée.
 * 
 * @param instance L'instance du problème du sac à dos à résoudre.
 * @param population_size La taille de la population pour l'algorithme génétique.
 * @param generations Le nombre de générations à exécuter.
 * @param mutation_rate Le taux de mutation pour l'algorithme génétique.
 * @param time_limit Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 * @param filename Le nom du fichier pour enregistrer les résultats.
 * 
 * @return Les résultats de l'algorithme, comprenant la valeur de la solution, le temps CPU et la taille de la solution.
 * 
 * @note Cette fonction exécute l'algorithme génétique sur l'instance donnée et retourne les résultats sous forme de structure `ResultEntry`.
 *      Elle mesure le temps CPU écoulé pendant l'exécution et calcule la taille de la solution.
 */
ResultEntry run_genetic_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int time_limit, const char *filename) ;

/**
 * @brief Exécute toutes les expériences sur une instance de sac à dos donnée.
 *
 * @param instance L'instance du problème du sac à dos à résoudre.
 * @param temps_max Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 * @param filename Le nom du fichier pour enregistrer les résultats.
 *
 * @return Les résultats expérimentaux, comprenant les valeurs de la solution et le temps CPU pour chaque étape.
 */
ExperimentalResultsKSM run_all_experiments(const KnapsackInstance *instance, int temps_max, const char *filename);

/**
 * @brief Affiche les résultats des expérimentations sous forme de tableau.
 *
 * @param results Les résultats expérimentaux à afficher (de type `ExperimentalResultsKSM`).
 */
void print_results_table(const ExperimentalResultsKSM *results);


/**
 * @brief Exporte les résultats expérimentaux dans un fichier CSV.
 *
 * @param results Les résultats expérimentaux à exporter (de type `ResultEntry`).
 * @param data_index L'index des données à exporter.
 * @param filename Le nom du fichier CSV dans lequel les résultats seront exportés.
 */
void export_csv(ResultEntry *results, int data_index, const char *filename);

/**
 * @brief Exécute les algorithmes VNS Gloutonne et VNS Aléatoire sur un répertoire d'instances de sac à dos.
 *
 * @param repertoire Le chemin du répertoire contenant les fichiers d'instances.
 * @param temps_max Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 * 
 * @note Cette fonction lit tous les fichiers d'instances dans le répertoire donné, exécute les algorithmes sur chaque instance,
 * * et exporte les résultats dans un fichier CSV.
 */
int vns_gloutonne_vs_aleatoire(const char *repertoire, int temps_max);

#endif // BENCHMARK_H
