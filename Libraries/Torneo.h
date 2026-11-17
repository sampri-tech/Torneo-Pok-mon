/*
 * La fase del torneo se deduce del numero de combate:
 *   1-48  -> fase de grupos
 *   49-64 -> fase eliminatoria
 */

#ifndef TORNEO_H
#define TORNEO_H

#include"Entrenador.h"
#include"Combate.h"
#include"LIST.h"

#define TORNEO_GROUPS 8
#define TORNEO_GROUP_SIZE 4
#define TORNEO_CLASSIFIED 16
#define TORNEO_GROUP_COMBATS 48
#define TORNEO_TOTAL_COMBATS 64

typedef struct Tournament Torneo;

/**
 * @brief Crea e inicializa un nuevo torneo.
 *
 * @return Torneo*: nuevo torneo, o NULL si falla la memoria.
 */
Torneo *NewTorneo(void);

/**
 * @brief Libera la memoria de un torneo.
 *
 * @param tour: torneo a destruir.
 * @note Si tour es NULL, no realiza ninguna acción.
 */
void DestroyTorneo(Torneo *tour);

/**
 * @brief Establece los entrenadores participantes del torneo.
 *
 * @param tour: torneo a actualizar.
 * @param trainers: arreglo de entrenadores.
 * @param count: cantidad de entrenadores.
 */
void SetTournamentTrainers(Torneo *tour, Trainer *trainers[], int count);

/**
 * @brief Obtiene la cantidad de entrenadores del torneo.
 *
 * @param tour: torneo consultado.
 * @return int: cantidad de entrenadores, o 0 si tour es NULL.
 */
int GetTournamentTrainerCount(const Torneo *tour);

/**
 * @brief Obtiene un entrenador del torneo por posición.
 *
 * @param tour: torneo consultado.
 * @param pos: posición del entrenador.
 * @return Trainer*: entrenador en la posición indicada, o NULL si no existe.
 * @note El puntero pertenece al arreglo de entrenadores; no debe liberarse.
 */
Trainer *GetTournamentTrainer(const Torneo *tour, int pos);

/**
 * @brief Genera los grupos del torneo con los entrenadores registrados.
 *
 * @param tour: torneo a actualizar.
 */
void GenerateGroups(Torneo *tour);

/**
 * @brief Genera los combates de la fase de grupos.
 *
 * @param tour: torneo a actualizar.
 */
void GenerateGroupMatches(Torneo *tour);

/**
 * @brief Genera la fase eliminatoria a partir de la clasificación de grupos.
 *
 * @param tour: torneo a actualizar.
 * @note Requiere que la fase de grupos esté completa.
 */
void GenerateElimination(Torneo *tour);

/**
 * @brief Obtiene el ID del entrenador azul de un combate.
 *
 * @param tour: torneo consultado.
 * @param numbCombat: número de combate.
 * @return int: ID del entrenador azul, o 0 si no existe.
 */
int GetMatchBlueId(const Torneo *tour, int numbCombat);

/**
 * @brief Obtiene el ID del entrenador rojo de un combate.
 *
 * @param tour: torneo consultado.
 * @param numbCombat: número de combate.
 * @return int: ID del entrenador rojo, o 0 si no existe.
 */
int GetMatchRedId(const Torneo *tour, int numbCombat);

/**
 * @brief Obtiene el ID del ganador de un combate.
 *
 * @param tour: torneo consultado.
 * @param numbCombat: número de combate.
 * @return int: ID del ganador, o 0 si no hay ganador/resultado.
 */
int GetMatchWinnerId(const Torneo *tour, int numbCombat);

/**
 * @brief Indica si un combate pertenece a la fase de grupos.
 *
 * @param numbCombat: número de combate.
 * @return int: 1 si es de grupos, 0 en caso contrario.
 */
int IsGroupCombat(int numbCombat);

/**
 * @brief Indica si un combate pertenece a la fase eliminatoria.
 *
 * @param numbCombat: número de combate.
 * @return int: 1 si es de eliminatoria, 0 en caso contrario.
 */
int IsEliminationCombat(int numbCombat);

/**
 * @brief Registra el resultado de un combate (carga por archivo o teclado).
 *
 * @param tour: torneo a actualizar.
 * @param numbCombat: número de combate.
 * @param winnerId: ID del ganador, o 0 para empate en fase de grupos.
 * @note Valida participantes, estado y reglas de fase; actualiza estadísticas y puede generar la eliminatoria.
 */
void SetMatchResult(Torneo *tour, int numbCombat, int winnerId);

/**
 * @brief Registra el resultado de un combate simulado en vivo.
 *
 * @param tour: torneo a actualizar.
 * @param numbCombat: número de combate.
 * @param winnerId: ID del ganador.
 * @note No actualiza estadísticas de entrenadores (ya actualizadas por la simulación); registra el resultado y avanza llaves.
 */
void SetLiveMatchResult(Torneo *tour, int numbCombat, int winnerId);

/**
 * @brief Indica si un combate ya tiene resultado registrado.
 *
 * @param tour: torneo consultado.
 * @param numbCombat: número de combate.
 * @return int: 1 si está jugado, 0 en caso contrario.
 */
int IsMatchPlayed(const Torneo *tour, int numbCombat);

/**
 * @brief Carga resultados desde un archivo de texto.
 *
 * @param tour: torneo a actualizar.
 * @param file: ruta del archivo.
 * @note Acepta líneas de 4 o 6 columnas; valida formato, participantes y compatibilidad con los combates.
 */
void LoadResultsFromFile(Torneo *tour, const char *file);

/**
 * @brief Guarda los resultados del torneo en un archivo de texto.
 *
 * @param tour: torneo consultado.
 * @param file: ruta del archivo de salida.
 */
void SaveResultsToFile(const Torneo *tour, const char *file);

/**
 * @brief Imprime los grupos del torneo.
 *
 * @param tour: torneo consultado.
 */
void PrintGroups(const Torneo *tour);

/**
 * @brief Imprime la información de un combate.
 *
 * @param tour: torneo consultado.
 * @param numbCombat: número de combate.
 */
void PrintMatch(const Torneo *tour, int numbCombat);

/**
 * @brief Imprime los combates del torneo.
 *
 * @param tour: torneo consultado.
 */
void PrintMatches(const Torneo *tour);

/**
 * @brief Imprime la clasificación del torneo.
 *
 * @param tour: torneo consultado.
 */
void PrintStandings(const Torneo *tour);

/**
 * @brief Guarda la clasificación del torneo en un archivo de texto.
 *
 * @param tour: torneo consultado.
 * @param file: ruta del archivo de salida.
 */
void SaveStandingsToFile(const Torneo *tour, const char *file);

/**
 * @brief Imprime el historial de resultados del torneo.
 *
 * @param tour: torneo consultado.
 */
void PrintHistory(const Torneo *tour);

/**
 * @brief Imprime los resultados finales del torneo.
 *
 * @param tour: torneo consultado.
 */
void PrintFinalResults(const Torneo *tour);

/**
 * @brief Obtiene el ID del campeón del torneo.
 *
 * @param tour: torneo consultado.
 * @return int: ID del campeón, o 0 si aún no hay campeón.
 */
int GetChampionId(const Torneo *tour);

#endif
