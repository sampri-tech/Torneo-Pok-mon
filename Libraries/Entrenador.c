#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Pokedex.h"
#include"Pokemon.h"
#include"Entrenador.h"
#include"LIST.h"
#define PKMNTEAM 6
#define MAXIDATTEMPTS 50
#define MINAVG 270

struct PkmonTrainer{
    int id; // Identificador unico del entrenador
    char name[20]; // Nombre del entrenador
    char prefTypes[2][20]; // Tipos favoritos; "-" si no hay filtro
    Specimens *team[PKMNTEAM]; // Ejemplares del equipo
    int tSize; // Cantidad actual de Pokemon en el equipo
    int points; // Puntos de clasificacion
    int wins; // Combates ganados
    int ties; // Combates empatados
    int defeats; // Combates perdidos
    int pkmDefeated; // Pokemon rivales derrotados
};

static int RandomTrainId(void){
    return ((rand() % 900) * 1000) + (rand() % 1000) + 100000;
}

static int IdExists(Trainer *trainers[], int count, int id){
    int i, found;

    found = 0;
    i = 0;

    if(trainers != NULL){
        while((i < count) && (found == 0)){
            if((trainers[i] != NULL) && (trainers[i]->id == id)){
                found = 1;
            }
            i++;
        }
    }

    return found;
}

static int GenUniqueId(Trainer *trainers[], int count){
    int id, tries;

    id = 0;
    tries = 0;

    while((tries < MAXIDATTEMPTS) && (id == 0)){
        id = RandomTrainId();

        if(IdExists(trainers, count, id)){
            id = 0;
        }

        tries++;
    }

    return id;
}

static int CmpType(const Species *sp, const Trainer *train){
    int i;

    i = 0;

    if((sp != NULL) && (train != NULL)){
        if(strcmp(train->prefTypes[0], GetFirstType(sp)) == 0){
            i++;
        }else if(GetSecondType(sp) != NULL){
            if(strcmp(train->prefTypes[0], GetSecondType(sp)) == 0) i++;
        }

        if(strcmp(train->prefTypes[1], "-") != 0){
            if(strcmp(train->prefTypes[1], GetFirstType(sp)) == 0){
                i++;
            }else if(GetSecondType(sp) != NULL){
                if(strcmp(train->prefTypes[1], GetSecondType(sp)) == 0) i++;
            }
        }
    }
    return i;
}

static void InterClear(Trainer *train){
    int i;

    if(train != NULL){
        i = 0;
        while(i < train->tSize){
            DestroySpecimen(train->team[i]);
            train->team[i] = NULL;
            i++;
        }

        train->tSize = 0;
    }
}

static int MaxBaseSum(const List *pkdex){
    const Species *sp;
    int i, max, sum;
    size_t size;

    max = 0;
    size = GetLSize(pkdex);

    for(i = 0; (size_t)i < size; i++){
        sp = SearchInPosition(pkdex, (size_t)i);

        if(sp != NULL){
            sum = GetBaseSum(sp);

            if(sum > max){
                max = sum;
            }
        }
    }

    return max;
}

static int GenTeam(const List *pkdex, const Trainer *train, int mate, int start,
                   const Species *team[], int tStats, int maxSum){
    const Species *candidate;
    int found, pos, total, sum;

    found = 0;

    if((pkdex != NULL) && (train != NULL) && (team != NULL) &&
       (mate >= 0) && (mate <= PKMNTEAM)){

        if(mate >= PKMNTEAM){
            if(tStats >= (PKMNTEAM * MINAVG)){
                found = 1;
            }
        }else if((tStats + ((PKMNTEAM - mate) * maxSum)) >=
                 (PKMNTEAM * MINAVG)){
            pos = start;
            total = (int)GetLSize(pkdex);

            while((found == 0) && (pos < total)){
                candidate = SearchInPosition(pkdex, (size_t)pos);

                if(candidate != NULL){
                    if((strcmp(train->prefTypes[0], "-") == 0) ||
                       (CmpType(candidate, train) > 0)){

                        team[mate] = candidate;
                        sum = GetBaseSum(candidate);

                        if(GenTeam(pkdex, train, mate + 1, pos + 1,
                                   team, tStats + sum, maxSum)){
                            found = 1;
                        }else{
                            team[mate] = NULL;
                        }
                    }
                }

                pos++;
            }
        }
    }

    return found;
}

static int CountPrefTypes(const List *pkdex, const Trainer *train){
    const Species *sp;
    int i, count;
    size_t size;

    count = 0;

    if((pkdex != NULL) && (train != NULL) &&
       (strcmp(train->prefTypes[0], "-") != 0)){
        size = GetLSize(pkdex);

        for(i = 0; (size_t)i < size; i++){
            sp = SearchInPosition(pkdex, (size_t)i);

            if((sp != NULL) && (CmpType(sp, train) > 0)){
                count++;
            }
        }
    }

    return count;
}

static int BuildTeam(const List *pkdex, const Trainer *train,
                     const Species *team[], int maxSum){
    int i;

    if(team != NULL){
        for(i = 0; i < PKMNTEAM; i++){
            team[i] = NULL;
        }
    }

    return GenTeam(pkdex, train, 0, 0, team, 0, maxSum);
}

static void ShufflePool(const Species *pool[], int total){
    const Species *aux;
    int i, j;

    if((pool != NULL) && (total > 1)){
        for(i = total - 1; i > 0; i--){
            j = rand() % (i + 1);
            aux = pool[i];
            pool[i] = pool[j];
            pool[j] = aux;
        }
    }
}

static int GenRandom(const Species *pool[], int total, const Trainer *train,
                     int mate, int start, const Species *team[],
                     int tStats, int maxSum, int pref, int minPref){
    const Species *candidate;
    int found, nextPref, pos, sum;

    found = 0;

    if((pool != NULL) && (train != NULL) && (team != NULL) &&
       (total >= 0) && (mate >= 0) && (mate <= PKMNTEAM) &&
       (start >= 0) && (start <= total) && (tStats >= 0) &&
       (maxSum >= 0) && (pref >= 0) && (minPref >= 0)){

        if(mate >= PKMNTEAM){
            if((tStats >= (PKMNTEAM * MINAVG)) && (pref >= minPref)){
                found = 1;
            }
        }else if((tStats + ((PKMNTEAM - mate) * maxSum)) >=
                 (PKMNTEAM * MINAVG)){
            if((pref + (PKMNTEAM - mate)) >= minPref){
                pos = start;

                while((found == 0) && (pos < total)){
                    candidate = pool[pos];

                    if(candidate != NULL){
                        team[mate] = candidate;
                        sum = GetBaseSum(candidate);
                        nextPref = pref;

                        if(CmpType(candidate, train) > 0){
                            nextPref++;
                        }

                        if(GenRandom(pool, total, train, mate + 1, pos + 1,
                                     team, tStats + sum, maxSum,
                                     nextPref, minPref)){
                            found = 1;
                        }else{
                            team[mate] = NULL;
                        }
                    }

                    pos++;
                }
            }
        }
    }

    return found;
}

static int RandomTeam(const List *pkdex, const Trainer *train,
                      const Species *team[], int maxSum, int minPref){
    const Species **pool;
    const Species *sp;
    int i, found, total;
    size_t size;

    found = 0;
    pool = NULL;

    if((pkdex != NULL) && (train != NULL) && (team != NULL) && (minPref >= 0)){
        for(i = 0; i < PKMNTEAM; i++){
            team[i] = NULL;
        }

        total = (int)GetLSize(pkdex);

        if(total >= PKMNTEAM){
            pool = (const Species **)malloc((size_t)total * sizeof(const Species *));

            if(pool != NULL){
                size = GetLSize(pkdex);
                i = 0;

                while((size_t)i < size){
                    sp = SearchInPosition(pkdex, (size_t)i);
                    pool[i] = sp;
                    i++;
                }

                ShufflePool(pool, total);
                found = GenRandom(pool, total, train, 0, 0, team, 0, maxSum, 0, minPref);
                free(pool);
            }
        }
    }

    return found;
}

static int InterAdd(Trainer *train, const Species *pkmn, int lvl){
    int i;

    i = 0;
    if((pkmn != NULL) && (train != NULL)){
        if(train->tSize < PKMNTEAM){
            train->team[train->tSize] = NewSpecimen(train->id, pkmn, lvl);
            if(train->team[train->tSize] != NULL){
                train->tSize++;
                i++;
            } 
        }
    }

    return i;
}

static void InterDel(Trainer *train, int i){
    int j;

    if((train != NULL) && (i >= 0) && (train->tSize > 0) && (i < train->tSize)){
        DestroySpecimen(train->team[i]);
        for(j = i; j < train->tSize - 1; j++){
            train->team[j] = train->team[j + 1];
        }
        train->team[train->tSize - 1] = NULL;
        train->tSize--;
    }
}

static int MemberLevel(int maxLvl, int pos){
    int base, rem, level;

    base = maxLvl / PKMNTEAM;
    rem = maxLvl % PKMNTEAM;
    level = base;

    if(pos < rem){
        level++;
    }

    if(level > 100){
        level = 100;
    }

    if(level < 1){
        level = 1;
    }

    return level;
}

static int StoreTeam(Trainer *train, const Species *team[], int maxLvl){
    Specimens *newTeam[PKMNTEAM];
    int i, stored, complete;

    stored = 0;

    if((train != NULL) && (team != NULL) && (maxLvl >= PKMNTEAM)){
        complete = 1;

        for(i = 0; i < PKMNTEAM; i++){
            newTeam[i] = NULL;

            if(team[i] != NULL){
                newTeam[i] = NewSpecimen(train->id, team[i], MemberLevel(maxLvl, i));
            }

            if(newTeam[i] == NULL){
                complete = 0;
            }
        }

        if(complete == 1){
            InterClear(train);

            for(i = 0; i < PKMNTEAM; i++){
                train->team[i] = newTeam[i];
            }

            train->tSize = PKMNTEAM;
            stored = 1;
        }else{
            for(i = 0; i < PKMNTEAM; i++){
                DestroySpecimen(newTeam[i]);
            }
        }
    }

    return stored;
}

static Trainer *InterNewTrainer(int id, const char *nam, const char *type1, const char *type2){
    Trainer *train;
    int i;

    if((id <= 0) || (nam == NULL) || ((type1 == NULL) && (type2 == NULL))){
        return NULL;
    }

    train = NULL;
    train = (Trainer *)malloc(sizeof(Trainer));

    if(train != NULL){
        snprintf(train->name, sizeof(train->name), "%s", nam);

        if((type1 != NULL) && (strcmp(type1, "-") != 0)){
            snprintf(train->prefTypes[0], sizeof(train->prefTypes[0]), "%s", type1);

            if((type2 != NULL) && (strcmp(type1, type2) != 0)){
                snprintf(train->prefTypes[1], sizeof(train->prefTypes[1]), "%s", type2);
            }else{
                strcpy(train->prefTypes[1], "-");
            }
        }else{
            if(((type1 == NULL) || (strcmp(type1, "-") == 0)) && (type2 != NULL)){
                snprintf(train->prefTypes[0], sizeof(train->prefTypes[0]), "%s", type2);
                strcpy(train->prefTypes[1], "-");
            }else{
                strcpy(train->prefTypes[0], "-");
                strcpy(train->prefTypes[1], "-");
            }
        }

        train->id = id;
        train->tSize = 0;

        for(i = 0; i < PKMNTEAM; i++){
            train->team[i] = NULL;
        }

        train->points = 0;
        train->wins = 0;
        train->ties = 0;
        train->defeats = 0;
        train->pkmDefeated = 0;
    }else{
        fprintf(stderr, "Error: fallo de memoria a la hora de crear un entrenador\n");
    }

    return train;
}

int AddTrainer(Trainer *trainers[], int *count, const char *nam, const char *type1, const char *type2){
    Trainer *train;
    int id, added;

    added = 0;

    if((trainers != NULL) && (count != NULL) && (*count >= 0) && (*count < MAXTRAIN)){
        id = GenUniqueId(trainers, *count);

        if(id > 0){
            train = InterNewTrainer(id, nam, type1, type2);

            if(train != NULL){
                trainers[*count] = train;
                (*count)++;
                added = 1;
            }
        }else{
            fprintf(stderr, "Error: no se pudo generar un ID unico.\n");
        }
    }else{
        fprintf(stderr, "Error: no se pueden agregar mas de %d entrenadores.\n", MAXTRAIN);
    }

    return added;
}

void ReadTrainers(const char *file, Trainer *trainers[], int *count){
    FILE *f;
    Trainer *train;
    char line[128], name[20], t1[20], t2[20];
    int id, n, hasId, reading;

    if((file != NULL) && (trainers != NULL) && (count != NULL) && (*count >= 0)){
        f = fopen(file, "r");

        if(f != NULL){
            reading = 1;

            while((reading == 1) && (fgets(line, sizeof(line), f) != NULL)){
                id = 0;
                hasId = 0;
                name[0] = '\0';
                strcpy(t1, "-");
                strcpy(t2, "-");

                n = sscanf(line, "%d %19s %19s %19s", &id, name, t1, t2);

                if(n >= 2){
                    hasId = 1;
                }else if((n == 0) || (n == EOF)){
                    name[0] = '\0';
                    strcpy(t1, "-");
                    strcpy(t2, "-");

                    sscanf(line, "%19s %19s %19s", name, t1, t2);
                }

                if(name[0] != '\0'){
                    if(*count >= MAXTRAIN){
                        fprintf(stderr, "Error: no se pueden cargar mas de %d entrenadores.\n", MAXTRAIN);
                        reading = 0;
                    }else if((hasId == 0) || ((id > 0) && (IdExists(trainers, *count, id) == 0))){
                        if(hasId == 0){
                            id = GenUniqueId(trainers, *count);
                        }

                        if(id == 0){
                            fprintf(stderr, "Error: no se pudo generar un ID unico.\n");
                            reading = 0;
                        }else{
                            train = InterNewTrainer(id, name, t1, t2);

                            if(train == NULL){
                                reading = 0;
                            }else{
                                trainers[*count] = train;
                                (*count)++;
                            }
                        }
                    }
                }
            }

            fclose(f);
        }else{
            fprintf(stderr, "Error: no se pudo abrir el archivo %s.\n", file);
        }
    }
}

const char *GetTrainerName(const Trainer *train){
    return (train != NULL) ? train->name : NULL;
}

const char *GetFirstPrefType(const Trainer *train){
    return (train != NULL) ? train->prefTypes[0]: NULL;
}

const char *GetSecPrefType(const Trainer *train){
    return (train != NULL) ? train->prefTypes[1]: NULL;
}

int GetTrainerId(const Trainer *train){
    return (train != NULL) ? train->id : 0;
}

int GetTeamSize(const Trainer *train){
    return (train != NULL) ? train->tSize : 0;
}

Specimens *GetTeamMember(const Trainer *train, int i){
    return ((train != NULL) && (i >= 0) && (i < train->tSize)) ? train->team[i] : NULL;
}

int GetWins(const Trainer *train){
    return (train != NULL) ? train->wins : 0;
}

int GetDefeats(const Trainer *train){
    return (train != NULL) ? train->defeats : 0;
}

int GetTies(const Trainer *train){
    return (train != NULL) ? train->ties : 0;
}

int GetPoints(const Trainer *train){
    return (train != NULL) ? train->points : 0;
}

int GetPkmDefeated(const Trainer *train){
    return (train != NULL) ? train->pkmDefeated : 0;
}

void AddWins(Trainer *train){
    if(train != NULL) train->wins++;
}

void AddTies(Trainer *train){
    if(train != NULL) train->ties++;
}

void AddDefeats(Trainer *train){
    if(train != NULL) train->defeats++;
}

void AddPoints(Trainer *train, int newPoints){
    if((train != NULL) && (newPoints > 0)){
        train->points += newPoints;
    }
}

void AddPkmDefeated(Trainer *train, int amount){
    if((train != NULL) && (amount > 0)){
        train->pkmDefeated += amount;
    }
}

int AddTeamMemb(Trainer *train, const Species *pkm, int lvl){
    return (InterAdd(train, pkm, lvl) > 0) ? 1 : 0;
}

void DelTeamMenb(Trainer *train, int i){
    InterDel(train, i);
}

void ClearTeam(Trainer *train){
    InterClear(train);
}

int GenerateTeam(const List *pkdex, Trainer *train, int maxLvl){
    const Species *team[PKMNTEAM];
    int i, generated, maxSum, minPref, prefCount;

    generated = 0;

    if((pkdex != NULL) && (train != NULL) && (maxLvl >= PKMNTEAM)){
        for(i = 0; i < PKMNTEAM; i++){
            team[i] = NULL;
        }

        maxSum = MaxBaseSum(pkdex);
        prefCount = CountPrefTypes(pkdex, train);
        minPref = 0;

        if(prefCount < PKMNTEAM){
            minPref = prefCount;
        }

        if(BuildTeam(pkdex, train, team, maxSum)){
            generated = StoreTeam(train, team, maxLvl);
        }else{
            if(RandomTeam(pkdex, train, team, maxSum, minPref)){
                generated = StoreTeam(train, team, maxLvl);
            }else if(minPref > 0){
                if(RandomTeam(pkdex, train, team, maxSum, 0)){
                    generated = StoreTeam(train, team, maxLvl);
                }
            }
        }
    }

    return generated;
}

void DestroyTrainer(Trainer *train){
    if(train != NULL){
        InterClear(train);
        free(train);
    }
}
