#include "benchmark.h"

double get_cpu_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

ResultEntry run_experiment(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *), int temps_max, const char *filename)
{
    ResultEntry result = {"", 0.0, 0.0, 0,"vnc", 0, 0.0, 0, 0, 0};
    
    double start_time = get_cpu_time();
    KnapsackSolution *solution = initialization_function(instance);
    variable_neighborhood_search(solution, instance, 100, 2, temps_max);
    double end_time = get_cpu_time();

    result.value = solution->Z;
    result.time = end_time - start_time;
    result.length = solution_length(solution, instance);
    strcpy(result.filename, filename);

    free_solution(solution);
    return result;
}

ResultEntry run_genetic_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int temps_max, const char *filename) {

    ResultEntry result = {"", 0.0, 0.0, 0,"genetic", 0, 0.0, 0, 0, 0};

    double start_time = get_cpu_time();
    KnapsackSolution *solution = genetic_algorithm(instance, population_size, generations, mutation_rate, temps_max);
    double end_time = get_cpu_time();

    result.value = solution->Z;
    result.time = end_time - start_time;
    result.length = solution_length(solution, instance);
    strcpy(result.filename, filename);

    free_solution(solution);
    return result;
}

ResultEntry run_hybrid_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int vns_iterations, int k, int temps_max, const char *filename) {

    ResultEntry result = {"", 0.0, 0.0, 0,"hybrid", 0, 0.0, 0, 0, 0};

    double start_time = get_cpu_time();
    KnapsackSolution *solution = hybrid_GA_VNS(instance, population_size, generations, mutation_rate, vns_iterations, k, temps_max);
    double end_time = get_cpu_time();

    result.value = solution->Z;
    result.time = end_time - start_time;
    result.length = solution_length(solution, instance);
    strcpy(result.filename, filename);

    free_solution(solution);
    return result;
}


ExperimentalResultsKSM run_all_experiments(const KnapsackInstance *instance, int temps_max, const char *filename)
{
    // principalement utiliser pour le main()
    ExperimentalResultsKSM results;
    results.greedy_vns = run_experiment(instance, greedy_initial_solution, temps_max, filename);
    results.random_vns = run_experiment(instance, random_initial_solution, temps_max, filename);

    results.genetic = run_genetic_algorithm(instance, 50, 100, 0.05, temps_max, filename);
    results.hybrid = run_hybrid_algorithm(instance, 50, 100, 0.05, 100, 2, temps_max, filename);
    return results;
}
void print_results_table(const ExperimentalResultsKSM *results)
{  
    // principalement utiliser dans le main()
    // Ligne de séparation complète avec la colonne Tailles
    printf("+------------------------------+-------------------------+-------------------------+----------+\n");

    // En-têtes des colonnes bien alignés
    printf("| %-28s | %-21s | %-21s | %-8s |\n",   "Combinaison", "Valeur de la solution", "Temps CPU (secondes)", "Tailles");

    // Ligne de séparation
    printf("+------------------------------+-------------------------+-------------------------+----------+\n");

    // Ligne Gloutonne - alignement parfait avec les en-têtes
    printf("| %-28s | %21.2f | %21.6f | %8d |\n",  "VNS Gloutonne",  results->greedy_vns.value, results->greedy_vns.time,  results->greedy_vns.length);

    // Ligne Aléatoire 
    printf("| %-28s | %21.2f | %21.6f | %8d |\n", "VNS Aléatoire",  results->random_vns.value,     results->random_vns.time,     results->random_vns.length);
    // Ligne Génétique

    printf("| %-28s | %21.2f | %21.6f | %8d |\n", "Génétique", results->genetic.value, results->genetic.time, results->genetic.length);
    // Ligne Hybride GA + VNS
    printf("| %-28s | %21.2f | %21.6f | %8d |\n", "Hybride GA + VNS", results->hybrid.value, results->hybrid.time, results->hybrid.length);

    // Ligne de séparation finale
    printf("+------------------------------+-------------------------+-------------------------+----------+\n");
}

int basic_test(int is_directory_mode, const char *path_instance, int temps_max)
{
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

                ExperimentalResultsKSM results = run_all_experiments(&ksInstance, temps_max, entry->d_name);
                print_results_table(&results);
            }
        }
        closedir(dir);
    }
    else
    {
        KnapsackInstance ksInstance;
        read_knapsack_file(path_instance, &ksInstance);

        ExperimentalResultsKSM results = run_all_experiments(&ksInstance, temps_max, path_instance);
        print_results_table(&results);
    }

    return 0;
}

void export_csv(ResultEntry *results, int data_index, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Erreur d'ouverture du fichier CSV");
        return;
    }

    // Écriture de l'en-tête
    fprintf(file, "filename,value,time,length,type,pop_size,mutation_rate,generations,vns_iterations,k\n");

    // Parcours du tableau de ResultEntry et écriture des données
    for (int i = 0; i < data_index; i++) {
        fprintf(file, "\"%s\",%lf,%lf,%d,\"%s\",%d,%lf,%d,%d,%d\n", 
                results[i].filename,
                results[i].value,
                results[i].time,
                results[i].length,
                results[i].type,
                results[i].pop_size,
                results[i].mutation_rate,
                results[i].generations,
                results[i].vns_iterations,
                results[i].k);
    }

    // Fermeture du fichier
    fclose(file);
    printf("Données exportées avec succès dans 'results.csv'.\n");
}

int vns_gloutonne_vs_aleatoire(const char *repertoire, int temps_max)
{
    printf("Test vns_gloutonne_vs_aleatoire.\n");
    // Ouvrir le répertoire
    DIR *dir = opendir(repertoire);
    if (!dir)
    {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    struct dirent *entry;
    // Tableau pour stocker les résultats à exporter
    ResultEntry results[SIZE_RESULT_BUFFER];
    int data_index = 0;

    // Lire les fichiers du répertoire
    while ((entry = readdir(dir)) != NULL)
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", repertoire, entry->d_name);

        struct stat st;
        // Vérifier si c'est un fichier régulier
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
        {
            printf("\n\n=== Instance : %s ===\n", entry->d_name);
            KnapsackInstance ksInstance;
            read_knapsack_file(full_path, &ksInstance);

            // Exécuter les expériences VNS Gloutonne et VNS Aléatoire
            ResultEntry greedy_result = run_experiment(&ksInstance, greedy_initial_solution, temps_max, entry->d_name);
            greedy_result.type = "vns_gloutonne";
            ResultEntry random_result = run_experiment(&ksInstance, random_initial_solution, temps_max, entry->d_name);
            random_result.type = "vns_aleatoire";

            // Sauvegarder les résultats dans le tableau
            results[data_index] = greedy_result;
            results[data_index + 1] = random_result;
            data_index += 2;
        }
    }

    closedir(dir);

    export_csv(results, data_index, "./benchmark/vnc_gloutonne_vs_aleatoire.csv");

    printf("Export des résultats terminé.\n");
    return 0;
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


    // return basic_test(is_directory_mode, path_instance, temps_max);

    // Les fonctions qui suivent on besoin de -D pour fonctionner
    return vns_gloutonne_vs_aleatoire(path_instance, temps_max); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire

}