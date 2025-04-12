#include "benchmark.h"

double get_cpu_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

ResultEntry run_experiment(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *), int temps_max)
{
    ResultEntry result;
    double start_time = get_cpu_time();
    KnapsackSolution *solution = initialization_function(instance);
    variable_neighborhood_search(solution, instance, 100, 2, temps_max);
    double end_time = get_cpu_time();

    result.value = solution->Z;
    result.time = end_time - start_time;
    result.length = solution_length(solution, instance);

    free_solution(solution);
    return result;
}

ExperimentalResultsKSM run_all_experiments(const KnapsackInstance *instance, int temps_max)
{
    ExperimentalResultsKSM results;
    results.greedy_vns = run_experiment(instance, greedy_initial_solution, temps_max);
    results.random_vns = run_experiment(instance, random_initial_solution, temps_max);
    return results;
}
void print_results_table(const ExperimentalResultsKSM *results)
{
    // Ligne de séparation complète avec la colonne Tailles
    printf("+------------------------------+-------------------------+-------------------------+----------+\n");

    // En-têtes des colonnes bien alignés
    printf("| %-28s | %-21s | %-21s | %-8s |\n",   "Combinaison", "Valeur de la solution", "Temps CPU (secondes)", "Tailles");

    // Ligne de séparation
    printf("+------------------------------+-------------------------+-------------------------+----------+\n");

    // Ligne Gloutonne - alignement parfait avec les en-têtes
    printf("| %-28s | %21.2f | %21.6f | %8d |\n",  "Gloutonne",  results->greedy_vns.value, results->greedy_vns.time,  results->greedy_vns.length);

    // Ligne Aléatoire - alignement cohérent
    printf("| %-28s | %21.2f | %21.6f | %8d |\n", "Aléatoire",  results->random_vns.value,     results->random_vns.time,     results->random_vns.length);

    // Ligne de séparation finale
    printf("+------------------------------+-------------------------+-------------------------+----------+\n");
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s [-D] <fichier_instance|répertoire> <temps_max>\n", argv[0]);
        printf("-Pour un fichier unique : %s <fichier_instance> <temps_max>\n", argv[0]);
        printf("-Pour un répertoire    : %s -D <repertoire_instance> <temps_max>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));
    int is_directory_mode = 0;
    char *path_instance;

    if (strcmp(argv[1], "-D") == 0)
    {
        if (argc < 4)
        {
            printf("Usage: %s -D <répertoire> <temps_max>\n", argv[0]);
            return 1;
        }
        is_directory_mode = 1;
        path_instance = argv[2];
    }
    else
    {
        path_instance = argv[1];
    }
    int temps_max = atof(argv[is_directory_mode ? 3 : 2]);

    if (is_directory_mode)
    {
        DIR *dir = opendir(path_instance);
        if (!dir)
        {
            perror("Erreur lors de l'ouverture du répertoire");
            return 1;
        }
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path_instance, entry->d_name);

            struct stat st;
            if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
            {
                printf("\n\n=== Instance : %s ===\n", entry->d_name);
                KnapsackInstance ksInstance;
                read_knapsack_file(full_path, &ksInstance);

                ExperimentalResultsKSM results = run_all_experiments(&ksInstance, temps_max);
                print_results_table(&results);
            }
        }
        closedir(dir);
    }
    else
    {
        KnapsackInstance ksInstance;
        read_knapsack_file(path_instance, &ksInstance);

        ExperimentalResultsKSM results = run_all_experiments(&ksInstance, temps_max);
        print_results_table(&results);
    }

    return 0;
}