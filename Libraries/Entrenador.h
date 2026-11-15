#ifndef ENTRENADOR_H
#define ENTRENADOR_H


#include "LIST.h"
#include "Pokemon.h"

#define MAXTRAIN 32

typedef struct PkmonTrainer Trainer;

int AddTrainer(Trainer *trainers[], int *count, const char *nam, const char *type1, const char *type2);
void ReadTrainers(const char *file, Trainer *trainers[], int *count);
int GenerateTeam(const List *pkdex, Trainer *train, int lvl);

const char *GetTrainerName(const Trainer *train);
const char *GetFirstPrefType(const Trainer *train);
const char *GetSecPrefType(const Trainer *train);
int GetTrainerId(const Trainer *train);
int GetTeamSize(const Trainer *train);
Specimens *GetTeamMember(const Trainer *train, int i);
int GetWins(const Trainer *train);
int GetDefeats(const Trainer *train);
int GetTies(const Trainer *train);
int GetPoints(const Trainer *train);
int GetPkmDefeated(const Trainer *train);

void AddWins(Trainer *train);
void AddTies(Trainer *train);
void AddDefeats(Trainer *train);
void AddPoints(Trainer *train, int newPoints);
void AddPkmDefeated(Trainer *train, int amount);
int AddTeamMemb(Trainer *train, const Species *pkm, int lvl);
void DelTeamMenb(Trainer *train, int i);
void ClearTeam(Trainer *train);

void DestroyTrainer(Trainer *train);


#endif
