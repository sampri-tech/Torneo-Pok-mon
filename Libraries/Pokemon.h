#ifndef POKEMON_H
#define POKEMON_H

#include "Pokedex.h"

typedef struct SpecimenPkm Specimens;

/**
 * @brief Crea un nuevo ejemplar de Pokémon a partir de una especie.
 *
 * @param trainerId: identificador del entrenador dueño.
 * @param sp: especie base del ejemplar.
 * @param lvl: nivel del ejemplar.
 * @return Specimens*: nuevo ejemplar, o NULL si los parámetros son inválidos o falla la memoria.
 * @note Las estadísticas se calculan a partir de la especie, el nivel y valores aleatorios.
 */
Specimens *NewSpecimen(int trainerId, const Species *sp, int lvl);

/**
 * @brief Obtiene el identificador único del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: identificador del ejemplar, o 0 si pkmon es NULL.
 */
int GetPkmnID(const Specimens *pkmon);

/**
 * @brief Obtiene el nivel del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: nivel del ejemplar, o 0 si pkmon es NULL.
 */
int GetLevel(const Specimens *pkmon);

/**
 * @brief Obtiene los puntos de vida actuales del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return float: vida actual, o 0.0f si pkmon es NULL.
 */
float GetActualHp(const Specimens *pkmon);

/**
 * @brief Obtiene los puntos de vida máximos del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return float: vida máxima, o 0.0f si pkmon es NULL.
 */
float GetMaxHp(const Specimens *pkmon);

/**
 * @brief Obtiene el ataque del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: ataque, o 0 si pkmon es NULL.
 */
int GetAtk(const Specimens *pkmon);

/**
 * @brief Obtiene la defensa del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: defensa, o 0 si pkmon es NULL.
 */
int GetDef(const Specimens *pkmon);

/**
 * @brief Obtiene la velocidad del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: velocidad, o 0 si pkmon es NULL.
 */
int GetSpd(const Specimens *pkmon);

/**
 * @brief Obtiene el nombre del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return const char*: nombre del ejemplar, o NULL si pkmon es NULL.
 * @note El string pertenece al ejemplar; no debe liberarse.
 */
const char *GetPkmName(const Specimens *pkmon);

/**
 * @brief Obtiene la especie del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return const Species*: especie asociada, o NULL si pkmon es NULL.
 * @note El puntero pertenece a la Pokédex; no debe liberarse.
 */
const Species *GetSpicie(const Specimens *pkmon);

/**
 * @brief Obtiene el primer tipo del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return const char*: primer tipo, o NULL si pkmon es NULL.
 * @note El string pertenece al ejemplar; no debe liberarse.
 */
const char *GetSpecimenFirstType(const Specimens *pkmon);

/**
 * @brief Obtiene el segundo tipo del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return const char*: segundo tipo, o NULL si pkmon es NULL o no tiene segundo tipo.
 * @note El string pertenece al ejemplar; no debe liberarse.
 */
const char *GetSpecimenSecondType(const Specimens *pkmon);

/**
 * @brief Obtiene el estado del ejemplar.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: 1 si no está derrotado, 0 si está derrotado o pkmon es NULL.
 */
int GetStatus(const Specimens *pkmon);

/**
 * @brief Indica si el ejemplar está debilitado.
 *
 * @param pkmon: ejemplar consultado.
 * @return int: 1 si está debilitado o pkmon es NULL, 0 en caso contrario.
 */
int IsFainted(const Specimens *pkmon);

/**
 * @brief Aplica daño al ejemplar defensor.
 *
 * @param def: ejemplar que recibe el daño.
 * @param dmg: cantidad de daño a aplicar.
 * @note Si el daño reduce la vida a 0 o menos, el ejemplar pasa a estado debilitado.
 */
void UpdateHp(Specimens *def, float dmg);

/**
 * @brief Restaura la vida y el estado del ejemplar.
 *
 * @param pkm: ejemplar a restaurar.
 * @note Si pkm es NULL, no realiza ninguna acción.
 */
void RestorePkmn(Specimens *pkm);

/**
 * @brief Libera la memoria de un ejemplar.
 *
 * @param pkmon: ejemplar a destruir.
 * @note Si pkmon es NULL, no realiza ninguna acción.
 */
void DestroySpecimen(Specimens *pkmon);

#endif
