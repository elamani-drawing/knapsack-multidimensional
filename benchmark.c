#include "benchmark.h"

double get_cpu_time()
{
    return (double)clock() / CLOCKS_PER_SEC;
}

ResultEntry run_experiment(const KnapsackInstance *instance, KnapsackSolution *(*initialization_function)(const KnapsackInstance *), int temps_max, int vns_iteration,  const char *filename, int k_perturbation)
{
    ResultEntry result = {"", 0.0, 0.0, 0,k_perturbation, "vnc", 0, 0.0, 0, vns_iteration};
    
    double start_time = get_cpu_time();
    KnapsackSolution *solution = initialization_function(instance);
    variable_neighborhood_search(solution, instance, vns_iteration, k_perturbation, temps_max);
    double end_time = get_cpu_time();

    result.value = solution->Z;
    result.time = end_time - start_time;
    result.length = solution_length(solution, instance);
    result.k_perturbation =k_perturbation;
    strcpy(result.filename, filename);

    free_solution(solution);
    return result;
}

ResultEntry run_genetic_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int temps_max, const char *filename) {

    ResultEntry result = {"", 0.0, 0.0, 0 ,0, "genetic", population_size, mutation_rate, generations, 0};

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

ResultEntry run_hybrid_algorithm(const KnapsackInstance *instance, int population_size, int generations, double mutation_rate, int vns_iterations, int k_perturbation, int temps_max, const char *filename) {

    ResultEntry result = {"", 0.0, 0.0, 0,k_perturbation, "genetic", population_size, mutation_rate, generations, vns_iterations};
    double start_time = get_cpu_time();
    KnapsackSolution *solution = hybrid_GA_VNS(instance, population_size, generations, mutation_rate, vns_iterations, k_perturbation, temps_max);
    double end_time = get_cpu_time();

    result.value = solution->Z;
    result.time = end_time - start_time;
    result.length = solution_length(solution, instance);
    strcpy(result.filename, filename);

    free_solution(solution);
    return result;
}


ExperimentalResultsKSM run_all_experiments(const KnapsackInstance *instance, int temps_max, const char *filename, int population_size, int generations, double mutation_rate,int vns_iteration, int k_perturbation)
{
    // principalement utiliser pour le main()
    ExperimentalResultsKSM results;
    results.greedy_vns = run_experiment(instance, greedy_initial_solution, temps_max, vns_iteration, filename, k_perturbation);
    results.random_vns = run_experiment(instance, random_initial_solution, temps_max, vns_iteration, filename, k_perturbation);

    results.genetic = run_genetic_algorithm(instance, population_size, generations, mutation_rate, temps_max, filename);
    results.hybrid = run_hybrid_algorithm(instance, population_size, generations, mutation_rate, vns_iteration, k_perturbation, temps_max, filename);
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

/**
 * @brief Exécute un test de base sur une instance de sac à dos ou un répertoire d'instances. (Suit les directives du main() qui ont ete mis en argument de l'executable)
 * 
 * @param is_directory_mode Indique si le chemin spécifié est un répertoire (1) ou un fichier (0).
 * @param path_instance Le chemin du fichier d'instance ou du répertoire contenant les instances.
 * @param temps_max Le temps maximum autorisé pour l'exécution de l'algorithme (en secondes).
 * @param population_size La taille de la population pour l'algorithme génétique.
 * @param generations Le nombre de générations à exécuter.
 *  @param mutation_rate Le taux de mutation pour l'algorithme génétique.
 * @param vns_iteration Le nombre d'itérations pour l'algorithme de recherche à voisinage variable (VNS).
 * @param k_perturbation Le paramètre de perturbation pour l'algorithme VNS.
 * 
 * @return 0 en cas de succès, 1 en cas d'erreur.
 * 
 * @note Cette fonction lit les fichiers d'instances dans le répertoire spécifié (ou un seul fichier) et exécute les algorithmes sur chaque instance.
 * * Les résultats sont affichés dans un tableau formaté.
 */
int basic_test(int is_directory_mode, const char *path_instance, int temps_max, int population_size, int generations, double mutation_rate, int vns_iteration, int k_perturbation)
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

                ExperimentalResultsKSM results = run_all_experiments(&ksInstance, temps_max, entry->d_name, population_size, generations, mutation_rate, vns_iteration, k_perturbation);
                print_results_table(&results);
            }
        }
        closedir(dir);
    }
    else
    {
        KnapsackInstance ksInstance;
        read_knapsack_file(path_instance, &ksInstance);

        ExperimentalResultsKSM results = run_all_experiments(&ksInstance, temps_max, path_instance,population_size, generations, mutation_rate, vns_iteration, k_perturbation);
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
    fprintf(file, "filename,value,time,length,k_perturbation,type,pop_size,mutation_rate,generations,vns_iterations\n");

    // Parcours du tableau de ResultEntry et écriture des données
    for (int i = 0; i < data_index; i++) {
        fprintf(file, "\"%s\",%lf,%lf,%d,%d,\"%s\",%d,%lf,%d,%d\n", 
                results[i].filename,
                results[i].value,
                results[i].time,
                results[i].length,
                results[i].k_perturbation,
                results[i].type,
                results[i].pop_size,
                results[i].mutation_rate,
                results[i].generations,
                results[i].vns_iterations);
    }

    // Fermeture du fichier
    fclose(file);
    printf("Données exportées avec succès.\n");
}

int vns_gloutonne_vs_aleatoire(const char *repertoire, int temps_max, int iteration, int k_perturbation)
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
            ResultEntry greedy_result = run_experiment(&ksInstance, greedy_initial_solution, temps_max,iteration,  entry->d_name, k_perturbation);
            greedy_result.type = "vns_gloutonne";
            ResultEntry random_result = run_experiment(&ksInstance, random_initial_solution, temps_max,iteration,  entry->d_name, k_perturbation);
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

int vns_gloutonne_vs_aleatoire_vns_iteration(const char *repertoire, const char *fichiers[], int num_fichiers, int k_perturbation) {
    printf("Test vns_gloutonne_vs_aleatoire_vns_iteration.\n");
    
    // Ouvrir le répertoire
    DIR *dir = opendir(repertoire);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    ResultEntry results[SIZE_RESULT_BUFFER];
    int data_index = 0;

    // Liste des itérations à tester
    int iterations[] = {100, 1000, 5000, 10000, 20000, 30000, 50000};

    // Vérifie les fichiers spécifiés
    for (int i = 0; i < num_fichiers; i++) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", repertoire, fichiers[i]);

        struct stat st;
        // Vérifie si c'est un fichier régulier
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode)) {
            printf("\n\n=== Instance : %s ===\n", fichiers[i]);
            KnapsackInstance ksInstance;
            read_knapsack_file(full_path, &ksInstance);

            // Boucle sur les différentes valeurs d'itérations
            for (size_t j = 0; j < sizeof(iterations)/sizeof(iterations[0]); j++) {
                int current_iteration = iterations[j];
                printf("Exécution pour %d itérations...\n", current_iteration);

                ResultEntry greedy_result = run_experiment(&ksInstance, greedy_initial_solution, 0, current_iteration, fichiers[i], k_perturbation);
                greedy_result.type = "vns_gloutonne";
                greedy_result.vns_iterations = current_iteration;

                ResultEntry random_result = run_experiment(&ksInstance, random_initial_solution, 0, current_iteration, fichiers[i], k_perturbation);
                random_result.type = "vns_aleatoire";
                random_result.vns_iterations = current_iteration;

                // Sauvegarder les résultats
                results[data_index] = greedy_result;
                results[data_index + 1] = random_result;
                data_index += 2;
            }
        }
    }

    closedir(dir);

    // Exporter les résultats dans un fichier CSV
    export_csv(results, data_index, "./benchmark/vns_gloutonne_vs_aleatoire_iteration.csv");

    printf("Export des résultats terminé.\n");
    return 0;
}


int vns_gloutonne_vs_aleatoire_time(const char *repertoire, const char *fichiers[], int num_fichiers, int iteration, int k_perturbation) {
    printf("Test vns_gloutonne_vs_aleatoire_time.\n");
    
    // Ouvrir le répertoire
    DIR *dir = opendir(repertoire);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    ResultEntry results[SIZE_RESULT_BUFFER];
    int data_index = 0;

    // Vérifie les fichiers spécifiés
    for (int i = 0; i < num_fichiers; i++) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", repertoire, fichiers[i]);

        struct stat st;
        // Vérifie si c'est un fichier régulier
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode)) {
            printf("\n\n=== Instance : %s ===\n", fichiers[i]);
            KnapsackInstance ksInstance;
            read_knapsack_file(full_path, &ksInstance);

            // Exécute les expériences pendant des périodes de 1 à 10 secondes
            for (int j = 1; j <= 10; j++) {
                printf("Exécution pour %d secondes...\n", j);
                ResultEntry greedy_result = run_experiment(&ksInstance, greedy_initial_solution, j,iteration, fichiers[i], k_perturbation);
                greedy_result.type = "vns_gloutonne";
                greedy_result.time = j; 
                ResultEntry random_result = run_experiment(&ksInstance, random_initial_solution, j, iteration, fichiers[i], k_perturbation);
                random_result.type = "vns_aleatoire";
                random_result.time = j; 

                // Sauvegarder les résultats dans le tableau
                results[data_index] = greedy_result;
                results[data_index + 1] = random_result;
                data_index += 2;
            }
        }
    }

    closedir(dir);

    // Exporter les résultats dans un fichier CSV
    export_csv(results, data_index, "./benchmark/vns_gloutonne_vs_aleatoire_time.csv");

    printf("Export des résultats terminé.\n");
    return 0;
}

int vns_gloutonne_vs_aleatoire_k_perturbation(const char *repertoire, const char *fichiers[], int num_fichiers, int iteration, int temps_max) {
    printf("Test vns_gloutonne_vs_aleatoire_k.\n");
    
    // Ouvrir le répertoire
    DIR *dir = opendir(repertoire);
    if (!dir) {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    ResultEntry results[SIZE_RESULT_BUFFER];
    int data_index = 0;

    // Vérifie les fichiers spécifiés
    for (int i = 0; i < num_fichiers; i++) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", repertoire, fichiers[i]);

        struct stat st;
        // Vérifie si c'est un fichier régulier
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode)) {
            printf("\n\n=== Instance : %s ===\n", fichiers[i]);
            KnapsackInstance ksInstance;
            read_knapsack_file(full_path, &ksInstance);

            
            for (int j = 0; j <= 50; j+=5) {
                printf("Exécution pour k = %d...\n", j);

                ResultEntry greedy_result = run_experiment(&ksInstance, greedy_initial_solution, temps_max, iteration, fichiers[i], j);
                greedy_result.type = "vns_gloutonne";

                ResultEntry random_result = run_experiment(&ksInstance, random_initial_solution, temps_max, iteration, fichiers[i], j);
                random_result.type = "vns_aleatoire";

                // Sauvegarder les résultats dans le tableau
                results[data_index] = greedy_result;
                results[data_index + 1] = random_result;
                data_index += 2;
            }
        }
    }

    closedir(dir);

    // Exporter les résultats dans un fichier CSV
    export_csv(results, data_index, "./benchmark/vns_gloutonne_vs_aleatoire_k_perturbation.csv");

    printf("Export des résultats terminé.\n");
    return 0;
}


int genetic_algorithm_tests(const char *chemin_fichier, int temps_max)
{
    printf("Test genetic_algorithm_tests.\n");

    // Ouvrir le fichier
    FILE *file = fopen(chemin_fichier, "r");
    if (!file)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    // Tableau pour stocker les résultats à exporter
    ResultEntry results[SIZE_RESULT_BUFFER];
    int data_index = 0;

    // Définir les paramètres du test
    int populations[] = {10, 100, 200, 400, 1000, 1300};
    int generations[] = {10, 50, 100, 300, 500,1000};
    double mutation_rates[] = {0.05, 0.1, 1,6, 10, 15, 20};

    // Lire le fichier (ici on suppose que le fichier contient une instance de knapsack)
    KnapsackInstance ksInstance;
    read_knapsack_file(chemin_fichier, &ksInstance);

    // Pour chaque combinaison de population, génération et taux de mutation
    for (size_t i = 0; i < sizeof(populations) / sizeof(populations[0]); i++) {
        for (size_t j = 0; j < sizeof(generations) / sizeof(generations[0]); j++) {
            for (size_t k = 0; k < sizeof(mutation_rates) / sizeof(mutation_rates[0]); k++) {
                printf("Exécution pour population %d, générations %d, taux de mutation %.2f...\n", populations[i], generations[j], mutation_rates[k]);
                // Exécuter l'algorithme génétique avec les paramètres
                ResultEntry result = run_genetic_algorithm(&ksInstance, populations[i], generations[j], mutation_rates[k], temps_max, chemin_fichier);
                result.pop_size = populations[i];
                result.generations = generations[j];
                result.mutation_rate = mutation_rates[k];

                // Sauvegarder les résultats dans le tableau
                results[data_index] = result;
                data_index++;
            }
        }
    }

    fclose(file);

    // Exporter les résultats dans un fichier CSV
    export_csv(results, data_index, "./benchmark/genetic_algorithm_tests.csv");

    printf("Export des résultats terminé.\n");
    return 0;
}

int run_hybrid_algorithm_test(const char *repertoire, int population_size, int generations, double mutation_rate, int vns_iterations, int k_perturbation, int temps_max)
{
    printf("Test run_hybrid_algorithm_test.\n");
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

            // Exécuter les expériences pour l'algorithme hybride et un autre algorithme
            ResultEntry hybrid_result = run_hybrid_algorithm(&ksInstance, population_size, generations, mutation_rate, vns_iterations, k_perturbation, temps_max, entry->d_name);
            hybrid_result.type = "hybrid";

            // Sauvegarder les résultats dans le tableau
            results[data_index] = hybrid_result;
            data_index += 1;
        }
    }

    closedir(dir);

    // Exporter les résultats dans un fichier CSV
    export_csv(results, data_index, "./benchmark/hybrid_ga_vns_test.csv");

    printf("Export des résultats terminé.\n");
    return 0;
}

int hybrid_vs_genetic_test(const char *repertoire, int population_size, int generations, double mutation_rate, int vns_iterations, int k_perturbation, int temps_max)
{
    printf("Test hybrid_vs_genetic_test.\n");

    // Ouvrir le répertoire
    DIR *dir = opendir(repertoire);
    if (!dir)
    {
        perror("Erreur lors de l'ouverture du répertoire");
        return 1;
    }

    struct dirent *entry;
    ResultEntry results[SIZE_RESULT_BUFFER];
    int data_index = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        // Ignorer les entrées spéciales . et ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", repertoire, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode))
        {
            printf("\n\n=== Instance : %s ===\n", entry->d_name);

            KnapsackInstance ksInstance;
            read_knapsack_file(full_path, &ksInstance);

            // Exécution de l'algorithme génétique
            ResultEntry genetic_result = run_genetic_algorithm(&ksInstance, population_size, generations, mutation_rate, temps_max, entry->d_name);
            genetic_result.type = "genetic";

            // Exécution de l'algorithme hybride (GA + VNS)
            ResultEntry hybrid_result = run_hybrid_algorithm(&ksInstance, population_size, generations, mutation_rate, vns_iterations, k_perturbation, temps_max, entry->d_name);
            hybrid_result.type = "hybrid";

            // Sauvegarder les résultats
            results[data_index] = genetic_result;
            results[data_index + 1] = hybrid_result;
            data_index += 2;
        }
    }

    closedir(dir);

    export_csv(results, data_index, "./benchmark/hybrid_vs_genetic_test.csv");

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
    // const char *fichiers[] = {"100M5_21.txt", "250M30_1.txt", "500M30_21.txt"};
    const char *fichiers[] = {"100M5_21.txt"};
    int num_fichiers = sizeof(fichiers) / sizeof(fichiers[0]);

    // meilleur config: population_size = 600, generations = 200, mutation_rate = 10, vns_iterations = 15000, k = 10, temps_max = 5
    int population_size = 600, generations =200, vns_iterations =15000, k_perturbation = 10;
    double mutation_rate = 10.0;

    int temps_max = atof(argv[is_directory_mode ? 3 : 2]);
    // return basic_test(is_directory_mode, path_instance, temps_max, population_size, generations, mutation_rate, vns_iterations, k_perturbation);
    
   
    // return genetic_algorithm_tests("Instances_MKP/250M5_11.txt", 5); // Décommenter pour exécuter le test de l'algorithme génétique sur un fichier

    // Necessite le parametre -D pour fonctionner

    // Exporter les donner sur vns gloutonne et aleatoire    $
    // iteration
    // return vns_gloutonne_vs_aleatoire_vns_iteration(path_instance, fichiers, num_fichiers, k_perturbation); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire avec des itérations de 100 à 50000
    // temps
    // return vns_gloutonne_vs_aleatoire_time(path_instance, fichiers, num_fichiers, vns_iterations, k_perturbation); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire avec des périodes de 1 à 10 secondes
    // return vns_gloutonne_vs_aleatoire_k_perturbation(path_instance, fichiers, num_fichiers, vns_iterations, 0); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire avec des valeurs de k de 10 à 100  
    // return vns_gloutonne_vs_aleatoire(path_instance, 5, vns_iterations, k_perturbation); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire
    
    // Necessite le parametre -D pour fonctionner
    // return run_hybrid_algorithm_test(path_instance, population_size, generations, 10, vns_iterations, k_perturbation, 5); // Décommenter pour exécuter le test de l'algorithme hybride sur un répertoire

    // Necessite le parametre -D pour fonctionner
    // return hybrid_vs_genetic_test(path_instance, population_size, generations, mutation_rate, vns_iterations, k_perturbation, temps_max); // Décommenter pour exécuter le test de l'algorithme hybride vs génétique sur un répertoire

    
    genetic_algorithm_tests("Instances_MKP/250M5_11.txt", 5); // Décommenter pour exécuter le test de l'algorithme génétique sur un fichier

    // Necessite le parametre -D pour fonctionner

    // Exporter les donner sur vns gloutonne et aleatoire    $
    // iteration
    vns_gloutonne_vs_aleatoire_vns_iteration(path_instance, fichiers, num_fichiers, k_perturbation); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire avec des itérations de 100 à 50000
    // temps
    vns_gloutonne_vs_aleatoire_time(path_instance, fichiers, num_fichiers, vns_iterations, k_perturbation); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire avec des périodes de 1 à 10 secondes
    vns_gloutonne_vs_aleatoire_k_perturbation(path_instance, fichiers, num_fichiers, vns_iterations, 0); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire avec des valeurs de k de 10 à 100  
    vns_gloutonne_vs_aleatoire(path_instance, 5, vns_iterations, k_perturbation); // Décommenter pour exécuter le test VNS Gloutonne vs Aléatoire sur un répertoire
    
    // Necessite le parametre -D pour fonctionner
    run_hybrid_algorithm_test(path_instance, population_size, generations, 10, vns_iterations, k_perturbation, 5); // Décommenter pour exécuter le test de l'algorithme hybride sur un répertoire

    // Necessite le parametre -D pour fonctionner
    return hybrid_vs_genetic_test(path_instance, population_size, generations, mutation_rate, vns_iterations, k_perturbation, temps_max); // Décommenter pour exécuter le test de l'algorithme hybride vs génétique sur un répertoire

}