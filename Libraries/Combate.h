#ifndef COMBATE_H
#define COMBATE_H

#include"Entrenador.h"
#include"LIST.h"

typedef struct ResultCombat Result;

/**
 * @brief Simula un combate entre dos entrenadores y registra el resultado en una lista.
 *
 * @param Combats: lista de resultados de combates.
 * @param blue: primer entrenador.
 * @param red: segundo entrenador.
 * @note No realiza la simulación si los parámetros son inválidos, los entrenadores son iguales o algún equipo está vacío.
 */
void Combat(List *Combats, Trainer *blue, Trainer *red);

/**
 * @brief Simula un combate en vivo entre dos entrenadores.
 *
 * @param blue: primer entrenador.
 * @param red: segundo entrenador.
 * @param numbCombat: número del combate.
 * @return int: ID del ganador, o 0 si hay empate o no se puede simular.
 * @note Actualiza las estadísticas de los entrenadores según el resultado del combate.
 */
int LiveCombat(Trainer *blue, Trainer *red, int numbCombat);

/**
 * @brief Obtiene el número de combate de un resultado.
 *
 * @param res: resultado consultado.
 * @return int: número de combate, o 0 si res es NULL.
 */
int GetNumCombat(Result *res);

/**
 * @brief Obtiene el ID del entrenador azul de un resultado.
 *
 * @param res: resultado consultado.
 * @return int: ID del entrenador azul, o 0 si res es NULL.
 */
int GetBlueId(Result *res);

/**
 * @brief Obtiene el ID del entrenador rojo de un resultado.
 *
 * @param res: resultado consultado.
 * @return int: ID del entrenador rojo, o 0 si res es NULL.
 */
int GetRedId(Result *res);

/**
 * @brief Obtiene el ID del ganador de un resultado.
 *
 * @param res: resultado consultado.
 * @return int: ID del ganador, o 0 si res es NULL o hubo empate.
 */
int GetWinnerId(Result *res);

/**
 * @brief Obtiene el ID del perdedor de un resultado.
 *
 * @param res: resultado consultado.
 * @return int: ID del perdedor, o 0 si res es NULL o hubo empate.
 */
int GetLoserId(Result *res);

/**
 * @brief Obtiene un resultado de combate desde una lista por posición.
 *
 * @param Combats: lista de resultados.
 * @param pos: posición del resultado.
 * @return Result*: puntero al resultado, o NULL si no existe o los parámetros son inválidos.
 * @note El puntero pertenece a la lista; no debe liberarse.
 */
Result *GetResultCombat(List *Combats, size_t pos);

/**
 * @brief Registra un resultado manual/predefinido entre dos entrenadores en una lista.
 *
 * @param Combats: lista de resultados.
 * @param blue: primer entrenador.
 * @param red: segundo entrenador.
 * @note No registra el resultado si los parámetros son inválidos, los entrenadores son iguales o algún equipo está vacío.
 */
void SetResult(List *Combats, Trainer *blue, Trainer *red);

#endif
