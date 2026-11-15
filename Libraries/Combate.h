#ifndef COMBATE_H
#define COMBATE_H

#include"Entrenador.h"
#include"LIST.h"

typedef struct ResultCombat Result;

void Combat(List *Combats, Trainer *blue, Trainer *red);
int LiveCombat(Trainer *blue, Trainer *red, int numbCombat);
int GetNumCombat(Result *res);
int GetBlueId(Result *res);
int GetRedId(Result *res);
int GetWinnerId(Result *res);
int GetLoserId(Result *res);
Result *GetResultCombat(List *Combats, size_t pos);
void SetResult(List *Combats, Trainer *blue, Trainer *red);

#endif
