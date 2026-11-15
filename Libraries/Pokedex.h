#ifndef POKEDEX_H
#define POKEDEX_H

#include "LIST.h"

typedef struct SpeciesPkmn Species;

/**
 * @brief Registra las especies de un archivo en una nueva Pokedex.
 *
 * @param file: ruta del archivo de texto con las especies.
 * @return List*: nueva lista con las especies registradas, o NULL si falla
 *         la creación de la lista.
 * @note Si file es NULL o no se puede abrir, devuelve una lista vacía.
 * @note Normaliza los tipos: si el primario no es válido y el secundario sí,
 *       se intercambian; si el secundario no es válido o es igual al primario,
 *       queda como "-". Las líneas sin ningún tipo válido se ignoran.
 */
List *RegistPokedex(const char *file);

/**
 * @brief Busca una especie por su nombre.
 *
 * @param pokedex: lista donde se busca.
 * @param name: nombre de la especie.
 * @return const Species*: puntero a la especie encontrada, o NULL si no existe
 *         o si algún parámetro es inválido.
 * @note El puntero pertenece a la lista; no debe liberarse.
 */
const Species *SearchSpecies(const List *pokedex, const char *name);

/**
 * @brief Busca una especie por su número de Pokedex.
 *
 * @param pokedex: lista donde se busca.
 * @param numb: número de la especie (1-based).
 * @return const Species*: puntero a la especie encontrada, o NULL si no existe
 *         o si algún parámetro es inválido.
 * @note El puntero pertenece a la lista; no debe liberarse.
 */
const Species *SearchNDex(const List *pokedex, int numb);

/**
 * @brief Busca todas las especies que tengan un tipo determinado.
 *
 * @param pokedex: lista donde se busca.
 * @param type: nombre del tipo a buscar.
 * @return List*: nueva lista con copias de las especies que coinciden, o NULL
 *         si falla la memoria.
 * @note La lista devuelta debe liberarse con DestroyList.
 * @note Si pokedex o type son NULL, devuelve una lista vacía.
 */
List *SearchByType(const List *pokedex, const char *type);

/**
 * @brief Obtiene la especie del siguiente nodo.
 *
 * @param pkmn: nodo actual.
 * @return const Species*: especie del siguiente nodo, o NULL si no hay
 *         siguiente o si pkmn es NULL.
 * @note El puntero pertenece a la lista; no debe liberarse.
 */
const Species *NextPkmn(const NodeL *pkmn);

/**
 * @brief Obtiene la especie del nodo anterior.
 *
 * @param pkmn: nodo actual.
 * @return const Species*: especie del nodo anterior, o NULL si no hay
 *         anterior o si pkmn es NULL.
 * @note El puntero pertenece a la lista; no debe liberarse.
 */
const Species *PrevPkmn(const NodeL *pkmn);

/**
 * @brief Obtiene el nombre de una especie.
 *
 * @param pkmn: especie consultada.
 * @return const char*: nombre de la especie, o NULL si pkmn es NULL.
 * @note El string pertenece a la especie; no debe liberarse.
 */
const char *GetSpeciesName(const Species *pkmn);

/**
 * @brief Obtiene el número de Pokédex de una especie.
 *
 * @param pkmn: especie consultada.
 * @return int: número de Pokédex, o 0 si pkmn es NULL.
 */
int GetSpeciesNumb(const Species *pkmn);

/**
 * @brief Obtiene la vida base de una especie.
 *
 * @param pkmn: especie consultada.
 * @return float: vida base, o 0.0f si pkmn es NULL.
 */
float GetBaseHp(const Species *pkmn);

/**
 * @brief Obtiene el ataque base de una especie.
 *
 * @param pkmn: especie consultada.
 * @return int: ataque base, o 0 si pkmn es NULL.
 */
int GetBaseAtk(const Species *pkmn);

/**
 * @brief Obtiene la defensa base de una especie.
 *
 * @param pkmn: especie consultada.
 * @return int: defensa base, o 0 si pkmn es NULL.
 */
int GetBaseDef(const Species *pkmn);

/**
 * @brief Obtiene la velocidad base de una especie.
 *
 * @param pkmn: especie consultada.
 * @return int: velocidad base, o 0 si pkmn es NULL.
 */
int GetBaseSpd(const Species *pkmn);

/**
 * @brief Obtiene el primer tipo de una especie.
 *
 * @param pkmn: especie consultada.
 * @return const char*: primer tipo, o NULL si pkmn es NULL.
 * @note El string pertenece a la especie; no debe liberarse.
 */
const char *GetFirstType(const Species *pkmn);

/**
 * @brief Obtiene el segundo tipo de una especie.
 *
 * @param pkmn: especie consultada.
 * @return const char*: segundo tipo, o NULL si pkmn es NULL o si la especie
 *         tiene un solo tipo.
 * @note El string pertenece a la especie; no debe liberarse.
 */
const char *GetSecondType(const Species *pkmn);

/**
 * @brief Obtiene la suma de las estadísticas base de una especie.
 *
 * @param pkmon: especie consultada.
 * @return int: suma de ataque, defensa, vida y velocidad, truncada a entero;
 *         0 si pkmon es NULL.
 */
int GetBaseSum(const Species *pkmon);

/**
 * @brief Imprime el número y el nombre de cada especie de la Pokedex.
 *
 * @param pkdex: Pokedex a imprimir.
 * @note Si pkdex es NULL no imprime nada; si está vacía, imprime un mensaje.
 */
void PrintPokedex(const List *pkdex);

/**
 * @brief Imprime los datos completos de una especie.
 *
 * @param pkmon: especie a imprimir.
 * @note Si pkmon es NULL, imprime "Especie no disponible.".
 */
void PrintSpecie(const Species *pkmon);

#endif
