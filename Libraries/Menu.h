#ifndef MENU_H
#define MENU_H

#include"Torneo.h"
#include"Pokedex.h"
#include"Entrenador.h"
#include"LIST.h"

/**
 * @brief Ejecuta el menú principal del torneo Pokémon.
 *
 * @param tour: torneo en uso.
 * @param pkdex: Pokédex cargada.
 * @param trainers: arreglo de entrenadores registrados.
 * @param count: puntero a la cantidad actual de entrenadores.
 * @note Controla la interacción por consola (Pokédex, entrenadores, resultados, torneo y salida).
 * @note Al salir guarda resultados y clasificación en archivos.
 */
void MenuRun(Torneo *tour, const List *pkdex, Trainer *trainers[], int *count);

#endif
