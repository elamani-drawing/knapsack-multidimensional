#include "chrono.h"

jmp_buf env;
volatile sig_atomic_t timeout_flag = 0;
LARGE_INTEGER start_time;

// Obtenir le temps actuel
TimeValue get_current_time() {
    TimeValue time;
#ifdef _WIN32
    QueryPerformanceCounter(&time);
#else
    clock_gettime(CLOCK_MONOTONIC, &time);
#endif
    return time;
}

// Calculer le temps écoulé en secondes
double get_elapsed_time(TimeValue start_time, TimeValue end_time) {
#ifdef _WIN32
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
#else
    return (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
#endif
}

// Vérifier si le temps est écoulé
void check_timeout(TimeValue start_time, double time_limit_seconds) {
    TimeValue current_time = get_current_time();
    double elapsed_time = get_elapsed_time(start_time, current_time);
    if (elapsed_time > time_limit_seconds) {
        timeout_flag = 1;
        longjmp(env, 1);  // Arrêter l'algorithme
    }
}
