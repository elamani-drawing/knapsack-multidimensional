#include "chrono.h"

jmp_buf env;
volatile sig_atomic_t timeout_flag = 0;

/**
 * @brief Stocke le temps de départ de la mesure.
 */
 #ifdef _WIN32
 LARGE_INTEGER start_time;
 #else
 struct timespec start_time;
 #endif // _WIN32
 

TimeValue get_current_time() {
    TimeValue time;
#ifdef _WIN32
    QueryPerformanceCounter(&time);
#else
    clock_gettime(CLOCK_MONOTONIC, &time);
#endif
    return time;
}


double get_elapsed_time(TimeValue start_time, TimeValue end_time) {
#ifdef _WIN32
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
#else
    return (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
#endif
}

void check_timeout(TimeValue start_time, double time_limit_seconds) {
    TimeValue current_time = get_current_time();
    double elapsed_time = get_elapsed_time(start_time, current_time);
    // printf("temps: %f sur %f\n",elapsed_time, time_limit_seconds);
    if (elapsed_time > time_limit_seconds) {
        timeout_flag = 1;
        longjmp(env, 1); 
    }
}
