#ifndef GENETIC_H
#define GENETIC_H

#include "heuristique.h"


/**
 * Représente un individu dans la population de l'algorithme génétique.
 * Un individu contient une solution au problème du sac à dos ainsi qu'une valeur de fitness associée à cette solution.
 * 
 * @param solution Un pointeur vers la solution (KnapsackSolution) associée à cet individu.
 * @param fitness La valeur de fitness de l'individu, qui évalue la qualité de la solution (ex. la valeur totale des objets dans le sac).
 */
 typedef struct {
    KnapsackSolution *solution; ///< Solution de l'individu (KnapsackSolution).
    double fitness;             ///< Fitness de la solution (évaluation de la qualité de la solution).
} Individual;

/**
 * @brief Initialisation d'un individu pour l'algorithme génétique.
 * 
 * Cette fonction alloue de la mémoire pour un individu, initialise sa solution
 * et calcule sa valeur de fitness en évaluant la solution. 
 * Si l'allocation échoue ou si l'initialisation de la solution échoue, 
 * la fonction renvoie `NULL`.
 *
 * @param n Le nombre d'éléments dans le problème du sac à dos.
 * @return Un pointeur vers l'individu initialisé, ou `NULL` en cas d'erreur.
 */
 Individual *init_individual(int n);

/**
 * Libère la mémoire allouée pour un individu.
 * 
 * @param individual Un pointeur vers l'individu à libérer.
 */
 void free_individual(Individual *individual);

 
 /**
 * Libère la mémoire d'une population d'individus.
 * 
 * @param population Un pointeur vers le tableau d'individus à libérer.
 * @param population_size La taille de la population.
 */
void free_population(Individual *population, int population_size) ;

/**
 * Effectue une sélection par tournoi entre deux individus de la population.
 * 
 * @param population Un tableau d'individus représentant la population.
 * @param population_size La taille de la population.
 * @return Un pointeur vers l'individu sélectionné, celui ayant la meilleure fitness parmi les deux sélectionnés.
 */
 Individual* tournament_selection(Individual *population, int population_size);

 /**
  * Effectue un croisement en un point entre deux parents pour générer un enfant.
  * Le croisement se fait en un seul point aléatoire.
  * 
  * @param parent1 Le premier parent.
  * @param parent2 Le second parent.
  * @param child Le pointeur où l'enfant généré sera stocké.
  * @param instance L'instance du problème de sac à dos, contenant des informations comme le nombre d'objets.
  */
 void crossover(KnapsackSolution *parent1, KnapsackSolution *parent2, KnapsackSolution *child, const KnapsackInstance *instance);
 
 
 /**
  * Effectue une mutation sur une solution donnée avec un taux de mutation donné.
  * La mutation consiste à inverser un bit au hasard dans la solution si le taux de mutation est respecté.
  * 
  * @param solution La solution à muter.
  * @param instance L'instance du problème de sac à dos, contenant des informations comme le nombre d'objets.
  * @param mutation_rate Le taux de mutation (probabilité de mutation).
  */
 void mutate(KnapsackSolution *solution, const KnapsackInstance *instance, double mutation_rate);
 
 
 /**
  * Exécute l'algorithme génétique pour résoudre le problème du sac à dos.
  * L'algorithme effectue les étapes suivantes :
  * - Initialisation de la population
  * - Sélection des parents par tournoi
  * - Croisement et mutation pour générer une nouvelle population
  * - Évaluation des solutions
  * Le processus est répété sur un nombre donné de générations.
  * 
  * @param instance L'instance du problème du sac à dos.
  * @param population_size La taille de la population.
  * @param generations Le nombre de générations à exécuter.
  * @param mutation_rate Le taux de mutation.
  * @return La meilleure solution trouvée à la fin des générations.
  */
 KnapsackSolution* genetic_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate);
 

#endif //GENETIC_H