#include<stdio.h>
#include<stdlib.h>
#include"Entrenador.h"
#include"Combate.h"
#include"LIST.h"
#include"Torneo.h"

struct Tournament{
    Trainer *trainers[MAXTRAIN]; // Entrenadores del torneo
    int trainerCount; // Cantidad de entrenadores registrados
    int group[TORNEO_GROUPS][TORNEO_GROUP_SIZE]; // IDs de los entrenadores por grupo
    int blueId[TORNEO_TOTAL_COMBATS + 1]; // ID del primer entrenador de cada combate
    int redId[TORNEO_TOTAL_COMBATS + 1]; // ID del segundo entrenador de cada combate
    int winnerId[TORNEO_TOTAL_COMBATS + 1]; // ID del ganador; 0 si no hay resultado o empate
    int played[TORNEO_TOTAL_COMBATS + 1]; // 1 si el combate ya tiene resultado
    int championId; // ID del campeon
    int runnerUpId; // ID del subcampeon
    int thirdId; // ID del tercer lugar
    int fourthId; // ID del cuarto lugar
};

static void BuildElimination(Torneo *tour);

static Trainer *FindTrainerById(const Torneo *tour, int id){
    Trainer *train;
    int i;

    train = NULL;
    i = 0;

    if((tour != NULL) && (id > 0)){
        while((i < tour->trainerCount) && (train == NULL)){
            if((tour->trainers[i] != NULL) && (GetTrainerId(tour->trainers[i]) == id)){
                train = tour->trainers[i];
            }
            i++;
        }
    }

    return train;
}

static void ResetMatches(Torneo *tour){
    int i;

    if(tour != NULL){
        for(i = 0; i <= TORNEO_TOTAL_COMBATS; i++){
            tour->blueId[i] = 0;
            tour->redId[i] = 0;
            tour->winnerId[i] = 0;
            tour->played[i] = 0;
        }
    }
}

static void SetMatch(Torneo *tour, int numbCombat, int blueId, int redId){
    if((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
        tour->blueId[numbCombat] = blueId;
        tour->redId[numbCombat] = redId;
        tour->winnerId[numbCombat] = 0;
        tour->played[numbCombat] = 0;
    }
}

static int GroupStageComplete(const Torneo *tour){
    int i, complete;

    complete = 1;
    i = 1;

    if(tour != NULL){
        while((i <= TORNEO_GROUP_COMBATS) && (complete == 1)){
            if(tour->played[i] == 0){
                complete = 0;
            }
            i++;
        }
    }else{
        complete = 0;
    }

    return complete;
}

static int CompareStanding(const Torneo *tour, int idA, int idB){
    Trainer *a, *b;
    int cmp;

    a = FindTrainerById(tour, idA);
    b = FindTrainerById(tour, idB);
    cmp = 0;

    if((a != NULL) && (b != NULL)){
        if(GetPoints(a) != GetPoints(b)){
            cmp = GetPoints(a) - GetPoints(b);
        }else if(GetWins(a) != GetWins(b)){
            cmp = GetWins(a) - GetWins(b);
        }else{
            cmp = GetPkmDefeated(a) - GetPkmDefeated(b);
        }
    }

    return cmp;
}

static void SortGroup(const Torneo *tour, int group, int order[]){
    int i, j, key;

    if((tour != NULL) && (order != NULL) && (group >= 0) && (group < TORNEO_GROUPS)){
        for(i = 0; i < TORNEO_GROUP_SIZE; i++){
            order[i] = tour->group[group][i];
        }

        for(i = 1; i < TORNEO_GROUP_SIZE; i++){
            key = order[i];
            j = i - 1;

            while((j >= 0) && (CompareStanding(tour, order[j], key) < 0)){
                order[j + 1] = order[j];
                j--;
            }

            order[j + 1] = key;
        }
    }
}

static int LoserId(const Torneo *tour, int numbCombat, int winnerId){
    int loserId;

    loserId = 0;

    if((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
        if(winnerId == tour->blueId[numbCombat]){
            loserId = tour->redId[numbCombat];
        }else if(winnerId == tour->redId[numbCombat]){
            loserId = tour->blueId[numbCombat];
        }
    }

    return loserId;
}

static void AdvanceWinner(Torneo *tour, int numbCombat, int winnerId){
    int loserId;

    if((tour != NULL) && (winnerId != 0)){
        loserId = LoserId(tour, numbCombat, winnerId);

        if(numbCombat == 49){
            tour->blueId[57] = winnerId;
        }else if(numbCombat == 50){
            tour->redId[57] = winnerId;
        }else if(numbCombat == 53){
            tour->blueId[58] = winnerId;
        }else if(numbCombat == 54){
            tour->redId[58] = winnerId;
        }else if(numbCombat == 51){
            tour->blueId[59] = winnerId;
        }else if(numbCombat == 52){
            tour->redId[59] = winnerId;
        }else if(numbCombat == 55){
            tour->blueId[60] = winnerId;
        }else if(numbCombat == 56){
            tour->redId[60] = winnerId;
        }else if(numbCombat == 57){
            tour->blueId[61] = winnerId;
        }else if(numbCombat == 58){
            tour->redId[61] = winnerId;
        }else if(numbCombat == 59){
            tour->blueId[62] = winnerId;
        }else if(numbCombat == 60){
            tour->redId[62] = winnerId;
        }else if(numbCombat == 61){
            tour->blueId[64] = winnerId;
            tour->blueId[63] = loserId;
        }else if(numbCombat == 62){
            tour->redId[64] = winnerId;
            tour->redId[63] = loserId;
        }else if(numbCombat == 63){
            tour->thirdId = winnerId;
            tour->fourthId = loserId;
        }else if(numbCombat == 64){
            tour->championId = winnerId;
            tour->runnerUpId = loserId;
        }
    }
}

static void ApplyMatchResult(Torneo *tour, int numbCombat, int winnerId, int updateStats,
                             int blueDefeated, int redDefeated){
    Trainer *blue, *red, *win, *lose;
    int blueId, redId, loserId;

    if((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
        blueId = tour->blueId[numbCombat];
        redId = tour->redId[numbCombat];

        if((blueId == 0) || (redId == 0)){
            fprintf(stderr, "Error: el combate %d no tiene participantes.\n", numbCombat);
        }else if(tour->played[numbCombat] == 1){
            fprintf(stderr, "Error: el combate %d ya tiene resultado.\n", numbCombat);
        }else if((winnerId != 0) && (winnerId != blueId) && (winnerId != redId)){
            fprintf(stderr, "Error: el ganador no pertenece al combate %d.\n", numbCombat);
        }else if((numbCombat > TORNEO_GROUP_COMBATS) && (winnerId == 0)){
            fprintf(stderr, "Error: la fase eliminatoria no permite empates.\n");
        }else{
            blue = FindTrainerById(tour, blueId);
            red = FindTrainerById(tour, redId);

            if((blue != NULL) && (red != NULL)){
                if(winnerId == 0){
                    if(updateStats == 1){
                        AddTies(blue);
                        AddTies(red);
                        AddPoints(blue, 1);
                        AddPoints(red, 1);

                        if(numbCombat <= TORNEO_GROUP_COMBATS){
                            AddPkmDefeated(blue, blueDefeated);
                            AddPkmDefeated(red, redDefeated);
                        }
                    }

                    tour->winnerId[numbCombat] = 0;
                    tour->played[numbCombat] = 1;

                    if((numbCombat <= TORNEO_GROUP_COMBATS) && (tour->blueId[49] == 0) &&
                       (GroupStageComplete(tour) == 1)){
                        BuildElimination(tour);
                    }
                }else{
                    win = FindTrainerById(tour, winnerId);
                    loserId = LoserId(tour, numbCombat, winnerId);
                    lose = FindTrainerById(tour, loserId);

                    if((win != NULL) && (lose != NULL)){
                        if(updateStats == 1){
                            AddWins(win);
                            AddDefeats(lose);
                            AddPoints(win, 3);

                            if(numbCombat <= TORNEO_GROUP_COMBATS){
                                AddPkmDefeated(blue, blueDefeated);
                                AddPkmDefeated(red, redDefeated);
                            }
                        }

                        tour->winnerId[numbCombat] = winnerId;
                        tour->played[numbCombat] = 1;
                        AdvanceWinner(tour, numbCombat, winnerId);

                        if((numbCombat <= TORNEO_GROUP_COMBATS) && (tour->blueId[49] == 0) &&
                           (GroupStageComplete(tour) == 1)){
                            BuildElimination(tour);
                        }
                    }
                }
            }
        }
    }
}

static void PrintMatch(const Torneo *tour, int numbCombat){
    Trainer *blue, *red, *win;
    int blueId, redId, winnerId;

    if((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
        blueId = tour->blueId[numbCombat];
        redId = tour->redId[numbCombat];
        winnerId = tour->winnerId[numbCombat];
        blue = FindTrainerById(tour, blueId);
        red = FindTrainerById(tour, redId);
        win = FindTrainerById(tour, winnerId);

        printf("Combate %d: ", numbCombat);

        if((blue != NULL) && (red != NULL)){
            printf("%s vs %s", GetTrainerName(blue), GetTrainerName(red));

            if(tour->played[numbCombat] == 1){
                if(winnerId == 0){
                    printf(" -> Empate\n");
                }else if(win != NULL){
                    printf(" -> Gana %s\n", GetTrainerName(win));
                }
            }else{
                printf(" -> Pendiente\n");
            }
        }else{
            printf("Pendiente\n");
        }
    }
}

static void PrintGroup(const Torneo *tour, int group){
    Trainer *train;
    int slot;
    char letter;

    if((tour != NULL) && (group >= 0) && (group < TORNEO_GROUPS)){
        letter = (char)('A' + group);
        printf("Grupo %c\n", letter);

        for(slot = 0; slot < TORNEO_GROUP_SIZE; slot++){
            train = FindTrainerById(tour, tour->group[group][slot]);

            if(train != NULL){
                printf("  %d %s\n", GetTrainerId(train), GetTrainerName(train));
            }else{
                printf("  Sin asignar\n");
            }
        }
    }
}

Torneo *NewTorneo(void){
    Torneo *tour;
    int i, j;

    tour = NULL;
    tour = (Torneo *)malloc(sizeof(Torneo));

    if(tour != NULL){
        tour->trainerCount = 0;
        tour->championId = 0;
        tour->runnerUpId = 0;
        tour->thirdId = 0;
        tour->fourthId = 0;

        for(i = 0; i < MAXTRAIN; i++){
            tour->trainers[i] = NULL;
        }

        for(i = 0; i < TORNEO_GROUPS; i++){
            for(j = 0; j < TORNEO_GROUP_SIZE; j++){
                tour->group[i][j] = 0;
            }
        }

        ResetMatches(tour);
    }else{
        fprintf(stderr, "Error: fallo de memoria al crear el torneo.\n");
    }

    return tour;
}

void DestroyTorneo(Torneo *tour){
    if(tour != NULL){
        free(tour);
    }
}

void SetTournamentTrainers(Torneo *tour, Trainer *trainers[], int count){
    int i, j;

    if((tour != NULL) && (trainers != NULL) && (count >= 0) && (count <= MAXTRAIN)){
        tour->trainerCount = count;

        for(i = 0; i < MAXTRAIN; i++){
            if(i < count){
                tour->trainers[i] = trainers[i];
            }else{
                tour->trainers[i] = NULL;
            }
        }

        for(i = 0; i < TORNEO_GROUPS; i++){
            for(j = 0; j < TORNEO_GROUP_SIZE; j++){
                tour->group[i][j] = 0;
            }
        }

        tour->championId = 0;
        tour->runnerUpId = 0;
        tour->thirdId = 0;
        tour->fourthId = 0;
        ResetMatches(tour);
    }else{
        fprintf(stderr, "Error: cantidad de entrenadores invalida.\n");
    }
}

int GetTournamentTrainerCount(const Torneo *tour){
    return (tour != NULL) ? tour->trainerCount : 0;
}

Trainer *GetTournamentTrainer(const Torneo *tour, int pos){
    return ((tour != NULL) && (pos >= 0) && (pos < tour->trainerCount)) ? tour->trainers[pos] : NULL;
}

void GenerateGroups(Torneo *tour){
    int i, group, slot;

    if((tour != NULL) && (tour->trainerCount == MAXTRAIN)){
        ResetMatches(tour);
        tour->championId = 0;
        tour->runnerUpId = 0;
        tour->thirdId = 0;
        tour->fourthId = 0;

        for(i = 0; i < TORNEO_GROUPS; i++){
            for(slot = 0; slot < TORNEO_GROUP_SIZE; slot++){
                tour->group[i][slot] = 0;
            }
        }

        for(i = 0; i < tour->trainerCount; i++){
            group = i % TORNEO_GROUPS;
            slot = i / TORNEO_GROUPS;
            tour->group[group][slot] = GetTrainerId(tour->trainers[i]);
        }
    }else{
        fprintf(stderr, "Error: se requieren %d entrenadores para generar los grupos.\n", MAXTRAIN);
    }
}

void GenerateGroupMatches(Torneo *tour){
    int group, i, j, match;

    if((tour != NULL) && (tour->trainerCount == MAXTRAIN) && (tour->group[0][0] != 0)){
        ResetMatches(tour);
        match = 1;

        for(group = 0; group < TORNEO_GROUPS; group++){
            for(i = 0; i < TORNEO_GROUP_SIZE; i++){
                for(j = i + 1; j < TORNEO_GROUP_SIZE; j++){
                    SetMatch(tour, match, tour->group[group][i], tour->group[group][j]);
                    match++;
                }
            }
        }
    }else{
        fprintf(stderr, "Error: primero debe generar los grupos.\n");
    }
}

static void BuildElimination(Torneo *tour){
    int group, i, order[TORNEO_GROUP_SIZE];

    if((tour != NULL) && (tour->trainerCount == MAXTRAIN) && (GroupStageComplete(tour) == 1)){
        for(i = TORNEO_GROUP_COMBATS + 1; i <= TORNEO_TOTAL_COMBATS; i++){
            tour->blueId[i] = 0;
            tour->redId[i] = 0;
            tour->winnerId[i] = 0;
            tour->played[i] = 0;
        }

        for(group = 0; group < TORNEO_GROUPS; group++){
            SortGroup(tour, group, order);

            if(group == 0){
                SetMatch(tour, 49, order[0], 0);
                SetMatch(tour, 53, order[1], 0);
            }else if(group == 1){
                SetMatch(tour, 53, order[0], tour->blueId[53]);
                SetMatch(tour, 49, tour->blueId[49], order[1]);
            }else if(group == 2){
                SetMatch(tour, 50, order[0], 0);
                SetMatch(tour, 54, order[1], 0);
            }else if(group == 3){
                SetMatch(tour, 54, order[0], tour->blueId[54]);
                SetMatch(tour, 50, tour->blueId[50], order[1]);
            }else if(group == 4){
                SetMatch(tour, 51, order[0], 0);
                SetMatch(tour, 55, order[1], 0);
            }else if(group == 5){
                SetMatch(tour, 55, order[0], tour->blueId[55]);
                SetMatch(tour, 51, tour->blueId[51], order[1]);
            }else if(group == 6){
                SetMatch(tour, 52, order[0], 0);
                SetMatch(tour, 56, order[1], 0);
            }else if(group == 7){
                SetMatch(tour, 56, order[0], tour->blueId[56]);
                SetMatch(tour, 52, tour->blueId[52], order[1]);
            }
        }
    }
}

void GenerateElimination(Torneo *tour){
    if((tour != NULL) && (tour->trainerCount == MAXTRAIN) && (GroupStageComplete(tour) == 1)){
        BuildElimination(tour);
    }else{
        fprintf(stderr, "Error: la fase de grupos debe estar completa antes de generar la eliminatoria.\n");
    }
}

int GetMatchBlueId(const Torneo *tour, int numbCombat){
    return ((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)) ? tour->blueId[numbCombat] : 0;
}

int GetMatchRedId(const Torneo *tour, int numbCombat){
    return ((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)) ? tour->redId[numbCombat] : 0;
}

int GetMatchWinnerId(const Torneo *tour, int numbCombat){
    return ((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)) ? tour->winnerId[numbCombat] : 0;
}

int IsGroupCombat(int numbCombat){
    return ((numbCombat >= 1) && (numbCombat <= TORNEO_GROUP_COMBATS)) ? 1 : 0;
}

int IsEliminationCombat(int numbCombat){
    return ((numbCombat > TORNEO_GROUP_COMBATS) && (numbCombat <= TORNEO_TOTAL_COMBATS)) ? 1 : 0;
}

void SetMatchResult(Torneo *tour, int numbCombat, int winnerId){
    ApplyMatchResult(tour, numbCombat, winnerId, 1, 0, 0);
}

void SetLiveMatchResult(Torneo *tour, int numbCombat, int winnerId){
    ApplyMatchResult(tour, numbCombat, winnerId, 0, 0, 0);
}

int IsMatchPlayed(const Torneo *tour, int numbCombat){
    int played;

    played = 0;

    if((tour != NULL) && (numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
        played = tour->played[numbCombat];
    }

    return played;
}

void LoadResultsFromFile(Torneo *tour, const char *file){
    FILE *f;
    char line[128];
    int numbCombat, blueId, redId, winnerId, blueDefeated, redDefeated, fields;

    if((tour != NULL) && (file != NULL)){
        f = fopen(file, "r");

        if(f != NULL){
            while(fgets(line, sizeof(line), f) != NULL){
                blueDefeated = 0;
                redDefeated = 0;
                fields = sscanf(line, "%d %d %d %d %d %d", &numbCombat, &blueId, &redId,
                                &winnerId, &blueDefeated, &redDefeated);

                if((fields == 4) || (fields == 6)){
                    if((numbCombat >= 1) && (numbCombat <= TORNEO_TOTAL_COMBATS)){
                        if((tour->blueId[numbCombat] == blueId) && (tour->redId[numbCombat] == redId)){
                            ApplyMatchResult(tour, numbCombat, winnerId, 1,
                                             blueDefeated, redDefeated);
                        }else{
                            fprintf(stderr, "Error: participantes incompatibles en el combate %d.\n", numbCombat);
                        }
                    }else{
                        fprintf(stderr, "Error: numero de combate invalido: %d.\n", numbCombat);
                    }
                }else if(fields != 0){
                    fprintf(stderr, "Error: formato de resultado invalido.\n");
                }
            }

            fclose(f);
        }else{
            fprintf(stderr, "Error: no se pudo abrir el archivo %s.\n", file);
        }
    }
}

void SaveResultsToFile(const Torneo *tour, const char *file){
    FILE *f;
    int i, written;

    written = 0;

    if((tour != NULL) && (file != NULL)){
        f = fopen(file, "w");

        if(f != NULL){
            for(i = 1; i <= TORNEO_TOTAL_COMBATS; i++){
                if(tour->played[i] == 1){
                    fprintf(f, "%d %d %d %d\n", i, tour->blueId[i],
                            tour->redId[i], tour->winnerId[i]);
                    written++;
                }
            }

            fclose(f);
            printf("Resultados guardados: %d combates en %s\n", written, file);
        }else{
            fprintf(stderr, "Error: no se pudo crear el archivo %s.\n", file);
        }
    }
}

void PrintGroups(const Torneo *tour){
    int group;

    if(tour != NULL){
        for(group = 0; group < TORNEO_GROUPS; group++){
            PrintGroup(tour, group);
        }
    }
}

void PrintMatches(const Torneo *tour){
    int i;

    if(tour != NULL){
        for(i = 1; i <= TORNEO_TOTAL_COMBATS; i++){
            PrintMatch(tour, i);
        }
    }
}

void PrintStandings(const Torneo *tour){
    Trainer *train;
    int group, i, order[TORNEO_GROUP_SIZE];

    if(tour != NULL){
        for(group = 0; group < TORNEO_GROUPS; group++){
            SortGroup(tour, group, order);
            printf("Grupo %c\n", (char)('A' + group));
            printf("Pos ID Nombre V E D Pts PkmDer\n");

            for(i = 0; i < TORNEO_GROUP_SIZE; i++){
                train = FindTrainerById(tour, order[i]);

                if(train != NULL){
                    printf("%d %d %s %d %d %d %d %d\n",
                           i + 1,
                           GetTrainerId(train),
                           GetTrainerName(train),
                           GetWins(train),
                           GetTies(train),
                           GetDefeats(train),
                           GetPoints(train),
                           GetPkmDefeated(train));
                }
            }
        }
    }
}

void SaveStandingsToFile(const Torneo *tour, const char *file){
    FILE *f;
    Trainer *train;
    int group, i, order[TORNEO_GROUP_SIZE];

    if((tour != NULL) && (file != NULL)){
        f = fopen(file, "w");

        if(f != NULL){
            for(group = 0; group < TORNEO_GROUPS; group++){
                SortGroup(tour, group, order);
                fprintf(f, "Grupo %c\n", (char)('A' + group));
                fprintf(f, "Pos ID Nombre V E D Pts PkmDer\n");

                for(i = 0; i < TORNEO_GROUP_SIZE; i++){
                    train = FindTrainerById(tour, order[i]);

                    if(train != NULL){
                        fprintf(f, "%d %d %s %d %d %d %d %d\n",
                                i + 1,
                                GetTrainerId(train),
                                GetTrainerName(train),
                                GetWins(train),
                                GetTies(train),
                                GetDefeats(train),
                                GetPoints(train),
                                GetPkmDefeated(train));
                    }
                }
            }

            fclose(f);
            printf("Clasificacion guardada en %s\n", file);
        }else{
            fprintf(stderr, "Error: no se pudo crear el archivo %s.\n", file);
        }
    }
}

void PrintHistory(const Torneo *tour){
    int i, found;

    found = 0;

    if(tour != NULL){
        for(i = 1; i <= TORNEO_TOTAL_COMBATS; i++){
            if(tour->played[i] == 1){
                PrintMatch(tour, i);
                found = 1;
            }
        }
    }

    if(found == 0){
        printf("No hay combates registrados.\n");
    }
}

void PrintFinalResults(const Torneo *tour){
    Trainer *train;

    if(tour != NULL){
        train = FindTrainerById(tour, tour->championId);
        if(train != NULL){
            printf("Campeon: %s\n", GetTrainerName(train));
        }else{
            printf("Campeon: pendiente\n");
        }

        train = FindTrainerById(tour, tour->runnerUpId);
        if(train != NULL){
            printf("Subcampeon: %s\n", GetTrainerName(train));
        }else{
            printf("Subcampeon: pendiente\n");
        }

        train = FindTrainerById(tour, tour->thirdId);
        if(train != NULL){
            printf("Tercer lugar: %s\n", GetTrainerName(train));
        }else{
            printf("Tercer lugar: pendiente\n");
        }

        train = FindTrainerById(tour, tour->fourthId);
        if(train != NULL){
            printf("Cuarto lugar: %s\n", GetTrainerName(train));
        }else{
            printf("Cuarto lugar: pendiente\n");
        }
    }
}

int GetChampionId(const Torneo *tour){
    return (tour != NULL) ? tour->championId : 0;
}
