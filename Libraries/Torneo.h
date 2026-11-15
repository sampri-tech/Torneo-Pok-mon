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

Torneo *NewTorneo(void);
void DestroyTorneo(Torneo *tour);

void SetTournamentTrainers(Torneo *tour, Trainer *trainers[], int count);
int GetTournamentTrainerCount(const Torneo *tour);
Trainer *GetTournamentTrainer(const Torneo *tour, int pos);

void GenerateGroups(Torneo *tour);
void GenerateGroupMatches(Torneo *tour);
void GenerateElimination(Torneo *tour);

int GetMatchBlueId(const Torneo *tour, int numbCombat);
int GetMatchRedId(const Torneo *tour, int numbCombat);
int GetMatchWinnerId(const Torneo *tour, int numbCombat);
int IsGroupCombat(int numbCombat);
int IsEliminationCombat(int numbCombat);

void SetMatchResult(Torneo *tour, int numbCombat, int winnerId);
void SetLiveMatchResult(Torneo *tour, int numbCombat, int winnerId);
int IsMatchPlayed(const Torneo *tour, int numbCombat);
void LoadResultsFromFile(Torneo *tour, const char *file);
void SaveResultsToFile(const Torneo *tour, const char *file);

void PrintGroups(const Torneo *tour);
void PrintMatches(const Torneo *tour);
void PrintStandings(const Torneo *tour);
void SaveStandingsToFile(const Torneo *tour, const char *file);
void PrintHistory(const Torneo *tour);
void PrintFinalResults(const Torneo *tour);
int GetChampionId(const Torneo *tour);

#endif
