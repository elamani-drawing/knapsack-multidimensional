#ifndef CHRONO_H
#define CHRONO_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <time.h>
#endif // _WIN32

/**
 * @brief Buffer de saut utilisé pour gérer le timeout avec longjmp.
 */
extern jmp_buf env; 

/**
 * @brief Drapeau indiquant si un timeout a été atteint.
 */
extern volatile sig_atomic_t timeout_flag;

/**
 * @brief Stocke le temps de départ de la mesure.
 */
extern LARGE_INTEGER start_time; 

/**
 * @brief Type générique représentant une valeur de temps selon le système d'exploitation.
 */
#ifdef _WIN32
typedef LARGE_INTEGER TimeValue;
#else
typedef struct timespec TimeValue;
#endif // _WIN32

/**
 * @brief Récupère le temps actuel.
 * 
 * @return La valeur du temps actuel sous forme de TimeValue.
 */
TimeValue get_current_time();

/**
 * @brief Calcule le temps écoulé entre deux instants.
 * 
 * @param start_time Temps de départ.
 * @param end_time Temps d'arrêt.
 * @return Le temps écoulé en secondes.
 */
double get_elapsed_time(TimeValue start_time, TimeValue end_time);

/**
 * @brief Vérifie si le temps écoulé dépasse une limite et déclenche un timeout si nécessaire.
 * 
 * @param start_time Temps de départ de la mesure.
 * @param time_limit_seconds Durée maximale autorisée en secondes.
 */
void check_timeout(TimeValue start_time, double time_limit_seconds);

#endif // CHRONO_H