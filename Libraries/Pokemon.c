#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Pokemon.h"

struct SpecimenPkm{
    int pkmnID; // Numero de identificación del pokemon
    int lvl; // Nivel del pokemon
    char name[20]; // Nombre del pokemon, generalmente el mismo de la especie
    const Species *spicie; // Especie del pokemon
    char types[2][20]; // Tipos del ejemplar copiados de la especie
    float maxHp; // Puntos de vida maximo del pokemon
    float actHp; // Puntos de vida actuales del pokemon
    int atk; // Puntos de ataque del pokemon
    int def; // Puntos de defensa del pokemo
    int spd; // Puntos de velocidad del pokemon
    int status; // 1 = no derrotado, 0 = derrotado
};

static int randomInt(int min, int max){
    return rand() % (max - min + 1) + min;
}

static int nextSpecimenId = 0;

static int NextSpecimenId(void){
    nextSpecimenId++;
    return nextSpecimenId;
}

Specimens *NewSpecimen(int trainerId, const Species *sp, int lvl){
    Specimens *pkm;
    const char *name;
    const char *type1;
    const char *type2;

    pkm = NULL;

    if((sp != NULL) && (trainerId > 0) && (lvl >= 1) && (lvl <= 100)){
        pkm = (Specimens *)malloc(sizeof(Specimens));
        if(pkm != NULL){
            pkm->pkmnID = NextSpecimenId();

            name = GetSpeciesName(sp);
            if(name != NULL){
                snprintf(pkm->name, sizeof(pkm->name), "%s", name);
            }else{
                pkm->name[0] = '\0';
            }

            type1 = GetFirstType(sp);
            type2 = GetSecondType(sp);

            if(type1 != NULL){
                snprintf(pkm->types[0], sizeof(pkm->types[0]), "%s", type1);
            }else{
                pkm->types[0][0] = '\0';
            }

            if(type2 != NULL){
                snprintf(pkm->types[1], sizeof(pkm->types[1]), "%s", type2);
            }else{
                snprintf(pkm->types[1], sizeof(pkm->types[1]), "%s", "-");
            }

            pkm->lvl = lvl;
            pkm->spicie = sp;
            pkm->maxHp = (((2 * GetBaseHp(sp) + randomInt(0, 31)) * lvl) / 100) + 5;
            pkm->actHp = pkm->maxHp;
            pkm->atk = (((2 * GetBaseAtk(sp) + randomInt(0, 31)) * lvl) / 100) + 10;
            pkm->def = (((2 * GetBaseDef(sp) + randomInt(0, 31)) * lvl) / 100) + 10;
            pkm->spd = (((2 * GetBaseSpd(sp) + randomInt(0, 31)) * lvl) / 100) + 10;
            pkm->status = 1;
        }
    }

    return pkm;
}

int GetPkmnID(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->pkmnID : 0;
}

int GetLevel(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->lvl : 0;
}

float GetActualHp(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->actHp : 0.0f;
}

float GetMaxHp(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->maxHp : 0.0f;
}

int GetAtk(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->atk : 0;
}

int GetDef(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->def : 0;
}

int GetSpd(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->spd : 0;
}

const char *GetPkmName(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->name : NULL;
}

const Species *GetSpicie(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->spicie : NULL;
}

const char *GetSpecimenFirstType(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->types[0] : NULL;
}

const char *GetSpecimenSecondType(const Specimens *pkmon){
    if((pkmon == NULL) || (strcmp(pkmon->types[1], "-") == 0)){
        return NULL;
    }

    return pkmon->types[1];
}

int GetStatus(const Specimens *pkmon){
    return (pkmon != NULL) ? pkmon->status : 0;
}

int IsFainted(const Specimens *pkmon){
    return (pkmon == NULL || pkmon->status == 0) ? 1 : 0;
}

void UpdateHp(Specimens *def, float dmg){

    if((def != NULL) && (dmg > 0)){
        def->actHp -= dmg;
        if (def->actHp <= 0){
            def->status = 0;
            def->actHp = 0;
        }
    }
}

void RestorePkmn(Specimens *pkm){
    
    if(pkm != NULL){
        pkm->actHp = pkm->maxHp;
        pkm->status = 1;
    }
}

void DestroySpecimen(Specimens *pkmon){
    free(pkmon);
}