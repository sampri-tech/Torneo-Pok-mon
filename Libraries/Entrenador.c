#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Pokedex.h"
#include"Pokemon.h"
#include"Entrenador.h"
#include"LIST.h"
#define MAXTEAM 6
#define MAXTEMPS 50
#define TEAMTEMP 6
#define MINSTATS 270

struct PkmonTrainer{
    int id; // Identificador unico del entrenador
    char name[20]; // Nombre del entrenador
    char prefTypes[2][20]; // Tipos favoritos; "-" si no hay filtro
    Specimens *team[MAXTEAM]; // Ejemplares del equipo
    int tSize; // Cantidad actual de Pokemon en el equipo
    int totalLvl; // Suma de niveles del equipo
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

    while((tries < MAXTEMPS) && (id == 0)){
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

static void NormalizeTypes(char t1[20], char t2[20]){
    NormalizeType(t1);
    NormalizeType(t2);

    if((strcmp(t1, "-") == 0) && (strcmp(t2, "-") != 0)){
        strcpy(t1, t2);
        strcpy(t2, "-");
    }else if((strcmp(t1, "-") != 0) && (strcmp(t2, "-") != 0) && (strcmp(t1, t2) == 0)){
        strcpy(t2, "-");
    }
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
        train->totalLvl = 0;
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

static int GenTeam(const List *pkdex, const Trainer *train, int mate, int start, const Species *team[], int tStats, int maxSum){
    const Species *candidate;
    int found, pos, total, sum;

    found = 0;

    if((pkdex != NULL) && (train != NULL) && (team != NULL) && (mate >= 0) && (mate <= MAXTEAM)){
        if(mate >= MAXTEAM){
            if(tStats >= (MAXTEAM * MINSTATS)){
                found = 1;
            }
        }else if((tStats + ((MAXTEAM - mate) * maxSum)) >= (MAXTEAM * MINSTATS)){
            pos = start;
            total = (int)GetLSize(pkdex);

            while((found == 0) && (pos < total)){
                candidate = SearchInPosition(pkdex, (size_t)pos);

                if((candidate != NULL) && ((strcmp(train->prefTypes[0], "-") == 0) || (CmpType(candidate, train) > 0))){
                    team[mate] = candidate;
                    sum = GetBaseSum(candidate);

                    if(GenTeam(pkdex, train, mate + 1, pos, team, tStats + sum, maxSum)){
                        found = 1;
                    }else{
                        team[mate] = NULL;
                    }
                }

                pos++;
            }
        }
    }

    return found;
}

static int BuildTeam(const List *pkdex, const Trainer *train, const Species *team[], int maxSum){
    int i;

    if(team != NULL){
        for(i = 0; i < MAXTEAM; i++){
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

static int GenRandom(const Species *pool[], int total, const Trainer *train, int mate, int start, const Species *team[], int tStats, int maxSum){
    const Species *candidate;
    int found, pos, sum;

    found = 0;

    if((pool != NULL) && (train != NULL) && (team != NULL) && (total > 0) && (mate >= 0) && (mate <= MAXTEAM) && (start >= 0) && (start <= total) && (tStats >= 0) && (maxSum >= 0)){
        if(mate >= MAXTEAM){
            if(tStats >= (MAXTEAM * MINSTATS)){
                found = 1;
            }
        }else if((tStats + ((MAXTEAM - mate) * maxSum)) >= (MAXTEAM * MINSTATS)){
            pos = start;

            while((found == 0) && (pos < total)){
                candidate = pool[pos];

                if(candidate != NULL){
                    team[mate] = candidate;
                    sum = GetBaseSum(candidate);

                    if(GenRandom(pool, total, train, mate + 1, pos, team, tStats + sum, maxSum)){
                        found = 1;
                    }else{
                        team[mate] = NULL;
                    }
                }

                pos++;
            }
        }
    }

    return found;
}

static int RandomTeam(const List *pkdex, const Trainer *train, const Species *team[], int maxSum){
    const Species **pool;
    const Species *sp;
    int i, found, total;
    size_t size;

    found = 0;
    pool = NULL;

    if((pkdex != NULL) && (train != NULL) && (team != NULL)){
        for(i = 0; i < MAXTEAM; i++){
            team[i] = NULL;
        }

        total = (int)GetLSize(pkdex);

        if(total > 0){
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
                found = GenRandom(pool, total, train, 0, 0, team, 0, maxSum);
                free(pool);
            }
        }
    }

    return found;
}

static int InterAdd(Trainer *train, const Species *pkmn, int lvl){
    int added;

    added = 0;

    train->team[train->tSize] = NewSpecimen(train->id, pkmn, lvl);

    if(train->team[train->tSize] != NULL){
        train->tSize++;
        added = 1;
    }

    return added;
}

static void InterDel(Trainer *train, int i){
    int j;

    DestroySpecimen(train->team[i]);

    for(j = i; j < train->tSize - 1; j++){
        train->team[j] = train->team[j + 1];
    }

    train->team[train->tSize - 1] = NULL;
    train->tSize--;
}

static int MemberLevel(int maxLvl, int pos){
    int base, rem, level;

    base = maxLvl / MAXTEAM;
    rem = maxLvl % MAXTEAM;
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
    Specimens *newTeam[MAXTEAM];
    int i, size, stored, complete;

    stored = 0;

    if((train != NULL) && (team != NULL) && (train->tSize > 0) && (maxLvl >= train->tSize)){
        size = train->tSize;
        complete = 1;

        for(i = 0; i < size; i++){
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

            for(i = 0; i < size; i++){
                train->team[i] = newTeam[i];
                train->totalLvl += GetLevel(newTeam[i]);
            }

            train->tSize = size;
            stored = 1;
        }else{
            for(i = 0; i < size; i++){
                DestroySpecimen(newTeam[i]);
            }
        }
    }

    return stored;
}

static Trainer *InterNewTrainer(int id, const char *nam, const char *type1, const char *type2){
    Trainer *train;
    char t1[20], t2[20];
    int i;

    if((id <= 0) || (nam == NULL) || (nam[0] == '\0')){
        return NULL;
    }

    if(type1 != NULL){
        snprintf(t1, sizeof(t1), "%s", type1);
    }else{
        strcpy(t1, "-");
    }

    if(type2 != NULL){
        snprintf(t2, sizeof(t2), "%s", type2);
    }else{
        strcpy(t2, "-");
    }

    NormalizeTypes(t1, t2);

    train = NULL;
    train = (Trainer *)malloc(sizeof(Trainer));

    if(train != NULL){
        snprintf(train->name, sizeof(train->name), "%s", nam);
        snprintf(train->prefTypes[0], sizeof(train->prefTypes[0]), "%s", t1);
        snprintf(train->prefTypes[1], sizeof(train->prefTypes[1]), "%s", t2);

        train->id = id;
        train->tSize = 0;
        train->totalLvl = 0;

        for(i = 0; i < MAXTEAM; i++){
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

int AddTeamMemb(Trainer *train, const Species *pkm, int maxLvl){
    int added, level;

    added = 0;

    if((train != NULL) && (pkm != NULL) && ((train->tSize + 1) <= MAXTEAM) && (maxLvl >= (train->tSize + 1))){
        level = MemberLevel(maxLvl, train->tSize);

        if((train->totalLvl + level) <= maxLvl){
            if(InterAdd(train, pkm, level)){
                train->totalLvl += level;
                added = 1;
            }
        }
    }

    return added;
}

int DelTeamMemb(Trainer *train, int i, int maxLvl){
    int deleted, level;

    deleted = 0;

    if((train != NULL) && (maxLvl >= train->tSize) && (i >= 0) && (i < train->tSize)){
        level = 0;

        if(train->team[i] != NULL){
            level = GetLevel(train->team[i]);
        }

        InterDel(train, i);
        train->totalLvl -= level;

        if(train->totalLvl < 0){
            train->totalLvl = 0;
        }

        deleted = 1;
    }

    return deleted;
}

void ClearTeam(Trainer *train){
    InterClear(train);
}

int GenerateTeam(const List *pkdex, Trainer *train, int maxLvl){
    const Species *team[MAXTEAM];
    int i, j, count, generated, maxSum, oldSize;

    generated = 0;
    oldSize = 0;

    if(train != NULL){
        oldSize = train->tSize;
    }

    if((pkdex != NULL) && (train != NULL) && (maxLvl >= MAXTEAM)){
        for(i = 0; i < MAXTEAM; i++){
            team[i] = NULL;
        }

        maxSum = MaxBaseSum(pkdex);

        if((strcmp(train->prefTypes[0], "-") == 0) && (strcmp(train->prefTypes[1], "-") == 0)){
            i = 0;

            while((i < TEAMTEMP) && (generated == 0)){
                if(RandomTeam(pkdex, train, team, maxSum)){
                    count = 0;

                    for(j = 0; j < MAXTEAM; j++){
                        if(team[j] != NULL){
                            count++;
                        }
                    }

                    if(count > 0){
                        train->tSize = count;
                        generated = StoreTeam(train, team, maxLvl);
                    }
                }

                i++;
            }
        }else{
            if(BuildTeam(pkdex, train, team, maxSum)){
                count = 0;

                for(j = 0; j < MAXTEAM; j++){
                    if(team[j] != NULL){
                        count++;
                    }
                }

                if(count > 0){
                    train->tSize = count;
                    generated = StoreTeam(train, team, maxLvl);
                }
            }

            if(generated == 0){
                i = 0;

                while((i < TEAMTEMP) && (generated == 0)){
                    if(RandomTeam(pkdex, train, team, maxSum)){
                        count = 0;

                        for(j = 0; j < MAXTEAM; j++){
                            if(team[j] != NULL){
                                count++;
                            }
                        }

                        if(count > 0){
                            train->tSize = count;
                            generated = StoreTeam(train, team, maxLvl);
                        }
                    }

                    i++;
                }
            }
        }
    }

    if((train != NULL) && (generated == 0)){
        train->tSize = oldSize;
    }

    return generated;
}

void DestroyTrainer(Trainer *train){
    if(train != NULL){
        InterClear(train);
        free(train);
    }
}
