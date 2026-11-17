#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include"LIST.h"
#include"Pokedex.h"
#define PkmTypes 18

struct SpeciesPkmn{
    int  numb; // Número de pokedex de la espeicie de pokemon
    char name[20]; // Nombre de la especie de pokemon
    char types[2][20]; //Tipo principal y secundario de la especie de pokemon
    float hp; //  Puntos de vida base de la especie de pokemon
    int atk; //  Puntos de ataque base de la especie de pokemon
    int def; //  Puntos de defensa base de la especie de pokemon
    int spd; //  Puntos de velocidad base de la especie de pokemon
};


static int SameType(const char *a, const char *b){
    int same;

    same = 1;
    if(a == NULL || b == NULL){
        same = 0;
    }else{
        while((*a != '\0') && (*b != '\0') && (same == 1)){
            if(tolower((unsigned char)*a) != tolower((unsigned char)*b)){
                same = 0;
            }
            a++;
            b++;
        }
        if((*a != '\0') || (*b != '\0')){
            same = 0;
        }
    }

    return same;
}

static int ValidTypes(char type[]){
    int i, valid;
    static const char *types[PkmTypes] = {"Acero", "Agua", "Bicho", "Dragón", "Eléctrico", "Fantasma",
    "Fuego", "Hada", "Hielo", "Lucha", "Normal", "Planta",
    "Psíquico", "Roca", "Siniestro", "Tierra", "Veneno", "Volador"};

    i = 0;
    valid = 0;
    if(type != NULL){
        while((i < PkmTypes) && (valid == 0)){
            if(SameType(type, types[i]) == 1){
                strcpy(type, types[i]);
                valid = 1;
            }else{
                i++;
            }
        }
        if((valid == 0) && (strcmp(type, "-") == 0)) valid = 2;
    }

    return valid;
}

int NormalizeType(char *type){
    int result;

    result = ValidTypes(type);

    if((type != NULL) && (result == 0)){
        strcpy(type, "-");
    }

    return result;
}

List *RegistPokedex(const char *file){
    List *pokedex;
    Species pkmon;
    int aux1, aux2;
    size_t before;
    FILE *entry;

    pokedex = NewList();
    if(pokedex == NULL){
        fprintf(stderr, "Error: fallo de memoria al crear la Pokedex.\n");
        return NULL;
    }

    if(file == NULL){
        fprintf(stderr, "Error: nombre de archivo nulo.\n");
        return pokedex;
    }

    entry = fopen(file, "r");
    if(entry == NULL){
        fprintf(stderr, "Error: no se pudo abrir el archivo %s.\n", file);
        return pokedex;
    }

    while(fscanf(entry, "%d %19s %19s %19s %f %i %i %i", &pkmon.numb, pkmon.name, pkmon.types[0], pkmon.types[1], &pkmon.hp, &pkmon.atk, &pkmon.def, &pkmon.spd) == 8){

        aux1 = ValidTypes(pkmon.types[0]);
        aux2 = ValidTypes(pkmon.types[1]);

        if((pkmon.numb > 0) && (aux1 == 1 || aux2 == 1)){
            if(aux1 != 1){
                strcpy(pkmon.types[0], pkmon.types[1]);
                strcpy(pkmon.types[1], "-");
            }else if(aux2 != 1 || strcmp(pkmon.types[0], pkmon.types[1]) == 0){
                strcpy(pkmon.types[1], "-");
            }

            before = GetLSize(pokedex);
            Add(pokedex, &pkmon, sizeof(Species), before);

            if(GetLSize(pokedex) == before){
                fprintf(stderr, "Error: no se pudo agregar la especie %s.\n", pkmon.name);
            }
        }
    }

    fclose(entry);
    return pokedex;
}

const Species *SearchSpecies(const List *pokedex, const char *name){
    const NodeL *current;
    const Species *result;

    if(pokedex == NULL || name == NULL) return NULL;

    current = GetHead(pokedex);
    while(current != NULL){
        result = GetElement(current);
        if(result != NULL && strcmp(result->name, name) == 0){
            return result;
        }
        current = GetNext(current);
    }

    return NULL;
}

const Species *SearchNDex(const List *pokedex, int numb){
    const NodeL *current;
    const Species *result;

    if(pokedex == NULL || numb <= 0){
        return NULL;
    }

    current = GetHead(pokedex);
    while(current != NULL){
        result = GetElement(current);
        if(result != NULL && result->numb == numb){
            return result;
        }
        current = GetNext(current);
    }

    return NULL;
}

static int cmpType(const void *elem, const void *key){
    const Species *sp;
    const char *type;

    sp = (const Species *)elem;
    type = (const char *)key;

    return (sp != NULL && type != NULL && (strcmp(sp->types[0], type) == 0 || strcmp(sp->types[1], type) == 0)) ? 1 : 0;
}

List *SearchByType(const List *pokedex, const char *type){
    List *result;

    if(pokedex == NULL || type == NULL){
        result = NewList();
    }else{
        result = SearchElement(pokedex, (void *)type, cmpType);
    }

    return result;
}

const Species *NextPkmn(const NodeL *pkmn){
    return GetElement(GetNext(pkmn));
}

const Species *PrevPkmn(const NodeL *pkmn){
    return GetElement(GetPrev(pkmn));
}

const char *GetSpeciesName(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->name : NULL;
}

int GetSpeciesNumb(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->numb : 0;
}

float GetBaseHp(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->hp : 0.0f;
}

int GetBaseAtk(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->atk : 0;
}

int GetBaseDef(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->def : 0;
}

int GetBaseSpd(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->spd : 0;
}

const char *GetFirstType(const Species *pkmn){
    return (pkmn != NULL) ? pkmn->types[0] : NULL;
}

const char *GetSecondType(const Species *pkmn){
    if(pkmn == NULL) return NULL;
    if(strcmp(pkmn->types[1], "-") == 0) return NULL;

    return pkmn->types[1];
}

int GetBaseSum(const Species *pkmon){
    return (pkmon != NULL) ? (int)(pkmon->atk + pkmon->def + pkmon->hp + pkmon->spd) : 0;
}

void PrintPokedex(const List *pkdex){
    const NodeL *current;
    const Species *pkmn;

    if(pkdex != NULL){
        current = GetHead(pkdex);
        if(current != NULL){
            printf("------------------------\n");
            do{
                pkmn = GetElement(current);
                if(pkmn != NULL){
                    printf("| No%03i\t%s |\n", pkmn->numb, pkmn->name);
                    printf("------------------------\n");
                }
                current = GetNext(current);
            }while(current != NULL);
        }else{
            printf("La Pokedex esta vacia.\n");
        }
    }
}

void PrintSpecie(const Species *pkmon){
    if(pkmon != NULL){
        printf("No%03i | Especie: %s\n", pkmon->numb, pkmon->name);
        printf("Tipo(s): | %s | | %s |\n", pkmon->types[0], pkmon->types[1]);
        printf("Estadisticas Base:\n");
        printf("\tVida: %.0f\n", pkmon->hp);
        printf("\tAtaque: %i\n", pkmon->atk);
        printf("\tDefensa: %i\n", pkmon->def);
        printf("\tVelocidad: %i\n", pkmon->spd);
        printf("\n");
    }else{
        printf("Especie no disponible.\n");
    }
}
