#include "benchmark.h"

double get_cpu_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}


ExperimentalResultsKSM run_experiments(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *)) {
    ExperimentalResultsKSM results;
    
    // Mesurer le temps CPU et la valeur de la solution initiale
    double start_time = get_cpu_time();
    KnapsackSolution *initial_solution = initialization_function(instance);
    double end_time = get_cpu_time();
    results.initial_value = initial_solution->Z;
    results.initial_time = end_time - start_time;
    
    // Mesurer le temps CPU et la valeur après recherche locale (1-flip)
    start_time = get_cpu_time();
    local_search_1_flip(initial_solution, instance);
    end_time = get_cpu_time();
    results.ls_value = initial_solution->Z;
    results.ls_time = end_time - start_time;

    // Mesurer le temps CPU et la valeur après VND
    start_time = get_cpu_time();
    variable_neighborhood_descent(initial_solution, instance, 0);
    end_time = get_cpu_time();
    results.vnd_value = initial_solution->Z;
    results.vnd_time = end_time - start_time;

    // Mesurer le temps CPU et la valeur après VNS
    start_time = get_cpu_time();
    variable_neighborhood_search(initial_solution, instance, 100, 2, 0);
    end_time = get_cpu_time();
    results.vns_value = initial_solution->Z;
    results.vns_time = end_time - start_time;

    // Libérer la mémoire de la solution initiale
    free_solution(initial_solution);
    return results;
}


void print_results_table(const ExperimentalResultsKSM *results) {
    printf("+---------------------+---------------------+---------------------+\n");
    printf("| Étape               | Valeur de la solution | Temps CPU (secondes) |\n");
    printf("+---------------------+---------------------+---------------------+\n");
    printf("| Solution initiale   | %19.2f | %19.6f |\n", results->initial_value, results->initial_time);
    printf("| Après recherche LS  | %19.2f | %19.6f |\n", results->ls_value, results->ls_time);
    printf("| Après VND           | %19.2f | %19.6f |\n", results->vnd_value, results->vnd_time);
    printf("| Après VNS           | %19.2f | %19.6f |\n", results->vns_value, results->vns_time);
    printf("+---------------------+---------------------+---------------------+\n");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <fichier_instance> <temps_max>\n", argv[0]);
        return 1;
    }
    srand(time(NULL));

    KnapsackInstance ksInstance;
    read_knapsack_file(argv[1], &ksInstance);

    // Exécuter les expériences avec la fonction d'initialisation gloutonne
    printf("Résultats avec initialisation gloutonne :\n");
    ExperimentalResultsKSM results_greedy = run_experiments(&ksInstance, greedy_initial_solution);
    print_results_table(&results_greedy);

    // Exécuter les expériences avec la fonction d'initialisation aléatoire
    printf("\nRésultats avec initialisation aléatoire :\n");
    ExperimentalResultsKSM results_random = run_experiments(&ksInstance, random_initial_solution);
    print_results_table(&results_random);

    return 0;
}