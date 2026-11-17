#ifndef ENTRENADOR_H
#define ENTRENADOR_H

#include "LIST.h"
#include "Pokemon.h"

#define MAXTRAIN 32

typedef struct PkmonTrainer Trainer;

/**
 * @brief Registra un nuevo entrenador en el arreglo.
 *
 * @param trainers: arreglo de entrenadores.
 * @param count: puntero a la cantidad actual de entrenadores; se incrementa al registrar.
 * @param nam: nombre del entrenador.
 * @param type1: primer tipo favorito, o "-" si no tiene.
 * @param type2: segundo tipo favorito, o "-" si no tiene.
 * @return int: 1 si se registró correctamente, 0 en caso contrario.
 * @note Falla si los parámetros son inválidos, si se alcanzó MAXTRAIN o si no hay memoria.
 */
int AddTrainer(Trainer *trainers[], int *count, const char *nam, const char *type1, const char *type2);

/**
 * @brief Carga entrenadores desde un archivo de texto.
 *
 * @param file: ruta del archivo.
 * @param trainers: arreglo donde se almacenan los entrenadores.
 * @param count: puntero a la cantidad actual; se actualiza con los entrenadores cargados.
 * @note Si file es NULL o no se puede abrir, no modifica el estado.
 */
void ReadTrainers(const char *file, Trainer *trainers[], int *count);

/**
 * @brief Genera el equipo de un entrenador.
 *
 * @param pkdex: Pokédex de especies disponibles.
 * @param train: entrenador al que se le genera el equipo.
 * @param maxLvl: nivel total máximo del equipo.
 * @return int: 1 si se generó correctamente, 0 en caso contrario.
 * @note El equipo cumple las restricciones de tipos y estadísticas base requeridas; usa backtracking y fallback aleatorio según corresponda.
 */
int GenerateTeam(const List *pkdex, Trainer *train, int maxLvl);

/**
 * @brief Obtiene el nombre de un entrenador.
 *
 * @param train: entrenador consultado.
 * @return const char*: nombre del entrenador, o NULL si train es NULL.
 * @note El string pertenece al entrenador; no debe liberarse.
 */
const char *GetTrainerName(const Trainer *train);

/**
 * @brief Obtiene el primer tipo favorito de un entrenador.
 *
 * @param train: entrenador consultado.
 * @return const char*: primer tipo favorito, o NULL si train es NULL.
 * @note El string pertenece al entrenador; no debe liberarse.
 */
const char *GetFirstPrefType(const Trainer *train);

/**
 * @brief Obtiene el segundo tipo favorito de un entrenador.
 *
 * @param train: entrenador consultado.
 * @return const char*: segundo tipo favorito, o NULL si train es NULL.
 * @note El string pertenece al entrenador; no debe liberarse.
 */
const char *GetSecPrefType(const Trainer *train);

/**
 * @brief Obtiene el identificador de un entrenador.
 *
 * @param train: entrenador consultado.
 * @return int: ID del entrenador, o 0 si train es NULL.
 */
int GetTrainerId(const Trainer *train);

/**
 * @brief Obtiene la cantidad de miembros del equipo.
 *
 * @param train: entrenador consultado.
 * @return int: tamaño del equipo, o 0 si train es NULL.
 */
int GetTeamSize(const Trainer *train);

/**
 * @brief Obtiene un miembro del equipo por índice.
 *
 * @param train: entrenador consultado.
 * @param i: índice del miembro.
 * @return Specimens*: ejemplar en la posición indicada, o NULL si no existe o los parámetros son inválidos.
 * @note El puntero pertenece al entrenador; no debe liberarse.
 */
Specimens *GetTeamMember(const Trainer *train, int i);

/**
 * @brief Obtiene la cantidad de victorias.
 *
 * @param train: entrenador consultado.
 * @return int: victorias, o 0 si train es NULL.
 */
int GetWins(const Trainer *train);

/**
 * @brief Obtiene la cantidad de derrotas.
 *
 * @param train: entrenador consultado.
 * @return int: derrotas, o 0 si train es NULL.
 */
int GetDefeats(const Trainer *train);

/**
 * @brief Obtiene la cantidad de empates.
 *
 * @param train: entrenador consultado.
 * @return int: empates, o 0 si train es NULL.
 */
int GetTies(const Trainer *train);

/**
 * @brief Obtiene los puntos de clasificación.
 *
 * @param train: entrenador consultado.
 * @return int: puntos, o 0 si train es NULL.
 */
int GetPoints(const Trainer *train);

/**
 * @brief Obtiene la cantidad de Pokémon rivales derrotados.
 *
 * @param train: entrenador consultado.
 * @return int: Pokémon derrotados, o 0 si train es NULL.
 */
int GetPkmDefeated(const Trainer *train);

/**
 * @brief Incrementa en uno las victorias del entrenador.
 *
 * @param train: entrenador a actualizar.
 */
void AddWins(Trainer *train);

/**
 * @brief Incrementa en uno los empates del entrenador.
 *
 * @param train: entrenador a actualizar.
 */
void AddTies(Trainer *train);

/**
 * @brief Incrementa en uno las derrotas del entrenador.
 *
 * @param train: entrenador a actualizar.
 */
void AddDefeats(Trainer *train);

/**
 * @brief Suma puntos de clasificación al entrenador.
 *
 * @param train: entrenador a actualizar.
 * @param newPoints: puntos a agregar.
 */
void AddPoints(Trainer *train, int newPoints);

/**
 * @brief Suma Pokémon derrotados al entrenador.
 *
 * @param train: entrenador a actualizar.
 * @param amount: cantidad a agregar.
 */
void AddPkmDefeated(Trainer *train, int amount);

/**
 * @brief Añade un miembro al equipo del entrenador.
 *
 * @param train: entrenador a actualizar.
 * @param pkm: especie del nuevo miembro.
 * @param maxLvl: nivel total máximo del equipo.
 * @return int: 1 si se añadió correctamente, 0 en caso contrario.
 */
int AddTeamMemb(Trainer *train, const Species *pkm, int maxLvl);

/**
 * @brief Elimina un miembro del equipo del entrenador.
 *
 * @param train: entrenador a actualizar.
 * @param i: índice del miembro a eliminar.
 * @param maxLvl: nivel total máximo del equipo.
 * @return int: 1 si se eliminó correctamente, 0 en caso contrario.
 */
int DelTeamMemb(Trainer *train, int i, int maxLvl);

/**
 * @brief Elimina todos los miembros del equipo del entrenador.
 *
 * @param train: entrenador a actualizar.
 */
void ClearTeam(Trainer *train);

/**
 * @brief Libera la memoria de un entrenador y su equipo.
 *
 * @param train: entrenador a destruir.
 * @note Si train es NULL, no realiza ninguna acción.
 */
void DestroyTrainer(Trainer *train);

#endif
