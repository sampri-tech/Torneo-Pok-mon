#ifndef POKEMON_H
#define POKEMON_H

#include "Pokedex.h"

typedef struct SpecimenPkm Specimens;

Specimens *NewSpecimen(int trainerId, const Species *sp, int lvl);


int GetPkmnID(const Specimens *pkmon);
int GetLevel(const Specimens *pkmon);
float GetActualHp(const Specimens *pkmon);
float GetMaxHp(const Specimens *pkmon);
int GetAtk(const Specimens *pkmon);
int GetDef(const Specimens *pkmon);
int GetSpd(const Specimens *pkmon);
const char *GetPkmName(const Specimens *pkmon);
const Species *GetSpicie(const Specimens *pkmon);
const char *GetSpecimenFirstType(const Specimens *pkmon);
const char *GetSpecimenSecondType(const Specimens *pkmon);

int GetStatus(const Specimens *pkmon);
int IsFainted(const Specimens *pkmon);
void UpdateHp(Specimens *def, float dmg);
void RestorePkmn(Specimens *pkm);

void DestroySpecimen(Specimens *pkmon);

#endif
